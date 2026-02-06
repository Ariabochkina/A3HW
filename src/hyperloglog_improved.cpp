#include "hyperloglog_improved.hpp"
#include <cmath>

HyperLogLogImproved::HyperLogLogImproved(int B, const HashFuncGen* hash) : hll_(B, hash) {}

double HyperLogLogImproved::bias(std::size_t m, double raw_est) {
    const double th_lo = 2.5 * static_cast<double>(m);
    const double th_hi = 5.0 * static_cast<double>(m);
    if (raw_est <= th_lo || raw_est > th_hi) return 0.0;
    return (raw_est - th_lo) * 0.015;
}

double HyperLogLogImproved::estimate() const {
    double E_raw = hll_.raw();
    std::size_t m = hll_.m();
    std::size_t V = hll_.zeros();
    const double th_hi = 5.0 * static_cast<double>(m);
    if (E_raw < th_hi && V > 0)
        return static_cast<double>(m) * std::log(static_cast<double>(m) / static_cast<double>(V));
    double E = E_raw - bias(m, E_raw);
    const double lim = 4294967296.0 / 30.0;
    if (E > lim)
        return -4294967296.0 * std::log1p(-E / 4294967296.0);
    return E;
}
