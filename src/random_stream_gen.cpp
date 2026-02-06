#include "random_stream_gen.hpp"
#include <fstream>
#include <algorithm>

RandomStreamGen::RandomStreamGen(std::uint64_t seed) : rng_(static_cast<std::mt19937::result_type>(seed)) {}

void RandomStreamGen::set_seed(std::uint64_t seed) {
    rng_.seed(static_cast<std::mt19937::result_type>(seed));
}

void RandomStreamGen::generate(std::size_t n) {
    s_.clear();
    s_.reserve(n);
    std::uniform_int_distribution<std::size_t> len_dist(1, MAX_LEN);
    std::uniform_int_distribution<std::size_t> char_dist(0, A_SIZE - 1);
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t len = len_dist(rng_);
        std::string str;
        str.reserve(len);
        for (std::size_t j = 0; j < len; ++j)
            str += ALPHABET[char_dist(rng_)];
        s_.push_back(std::move(str));
    }
}

std::pair<std::vector<std::string>::const_iterator, std::vector<std::string>::const_iterator>
RandomStreamGen::prefix(double p) const {
    if (s_.empty()) return {s_.cend(), s_.cend()};
    p = std::clamp(p, 0.0, 1.0);
    std::size_t k = static_cast<std::size_t>(p * static_cast<double>(s_.size()));
    if (k > s_.size()) k = s_.size();
    return {s_.cbegin(), s_.cbegin() + k};
}

std::pair<std::vector<std::string>::const_iterator, std::vector<std::string>::const_iterator>
RandomStreamGen::prefix_n(std::size_t k) const {
    if (k >= s_.size()) return {s_.cbegin(), s_.cend()};
    return {s_.cbegin(), s_.cbegin() + k};
}

bool RandomStreamGen::save(const std::string& path) const {
    std::ofstream out(path);
    if (!out) return false;
    for (const auto& x : s_) out << x << '\n';
    return !!out;
}

std::size_t RandomStreamGen::load(const std::string& path) {
    s_.clear();
    std::ifstream in(path);
    if (!in) return 0;
    std::string line;
    while (std::getline(in, line)) s_.push_back(line);
    return s_.size();
}
