#include "hyperloglog.hpp"
#include <cmath>

#ifdef __GNUC__
#define CLZ32(x) __builtin_clz(x)
#else
static int CLZ32(std::uint32_t x) {
    if (x == 0) return 32;
    int n = 0;
    if (x <= 0x0000FFFF) { n += 16; x <<= 16; }
    if (x <= 0x00FFFFFF) { n += 8;  x <<= 8; }
    if (x <= 0x0FFFFFFF) { n += 4;  x <<= 4; }
    if (x <= 0x3FFFFFFF) { n += 2;  x <<= 2; }
    if (x <= 0x7FFFFFFF) n += 1;
    return n;
}
#endif

HyperLogLog::HyperLogLog(int B, const HashFuncGen* hash)
    : B_(B), m_(static_cast<std::size_t>(1) << B), hash_(hash) {
    M_.assign(m_, 0);
}

void HyperLogLog::add(std::string_view key) {
    if (!hash_) return;
    std::uint32_t w = (*hash_)(key);
    std::uint32_t mask = (1u << (32 - B_)) - 1u;
    std::size_t j = w >> (32 - B_);
    std::uint32_t tail = w & mask;
    int rho = (tail == 0) ? (32 - B_ + 1) : (1 + static_cast<int>(CLZ32(tail)) - B_);
    if (rho > static_cast<int>(M_[j]))
        M_[j] = static_cast<std::uint8_t>(rho);
}

double HyperLogLog::alpha(std::size_t m) {
    if (m == 16) return 0.673;
    if (m == 32) return 0.697;
    if (m == 64) return 0.709;
    return 0.7213 / (1.0 + 1.079 / static_cast<double>(m));
}

double HyperLogLog::raw() const {
    double sum = 0;
    for (std::size_t j = 0; j < m_; ++j)
        sum += std::exp2(-static_cast<double>(M_[j]));
    double Z = 1.0 / sum;
    return alpha(m_) * static_cast<double>(m_) * static_cast<double>(m_) * Z;
}

double HyperLogLog::estimate() const {
    double E = raw();
    std::size_t V = zeros();
    const double th_lo = 2.5 * static_cast<double>(m_);
    if (E < th_lo && V > 0)
        return static_cast<double>(m_) * std::log(static_cast<double>(m_) / static_cast<double>(V));
    const double lim = 4294967296.0 / 30.0;
    if (E > lim)
        return -4294967296.0 * std::log1p(-E / 4294967296.0);
    return E;
}

void HyperLogLog::reset() {
    std::fill(M_.begin(), M_.end(), 0);
}

std::size_t HyperLogLog::mem() const {
    return sizeof(*this) + M_.size() * sizeof(M_[0]);
}

std::size_t HyperLogLog::zeros() const {
    std::size_t v = 0;
    for (std::size_t j = 0; j < m_; ++j)
        if (M_[j] == 0) ++v;
    return v;
}
