#pragma once

#include <cstddef>
#include <random>
#include <string>
#include <vector>

class RandomStreamGen {
public:
    static constexpr int MAX_LEN = 30;
    static constexpr const char* ALPHABET =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-";

    explicit RandomStreamGen(std::uint64_t seed = 0);
    void set_seed(std::uint64_t seed);
    void generate(std::size_t n);
    std::size_t size() const { return s_.size(); }

    std::pair<std::vector<std::string>::const_iterator, std::vector<std::string>::const_iterator>
    prefix(double p) const;

    std::pair<std::vector<std::string>::const_iterator, std::vector<std::string>::const_iterator>
    prefix_n(std::size_t k) const;

    const std::vector<std::string>& stream() const { return s_; }
    bool save(const std::string& path) const;
    std::size_t load(const std::string& path);

private:
    std::mt19937 rng_;
    std::vector<std::string> s_;
    static constexpr std::size_t A_SIZE = 63;
};
