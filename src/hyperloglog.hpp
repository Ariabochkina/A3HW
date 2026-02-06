#pragma once

#include "hash_func_gen.hpp"
#include <cstddef>
#include <cstdint>
#include <unordered_set>
#include <vector>

template <typename It>
std::size_t exact_count(It begin, It end) {
    using T = typename std::iterator_traits<It>::value_type;
    std::unordered_set<T> set(begin, end);
    return set.size();
}

class HyperLogLog {
public:
    explicit HyperLogLog(int B, const HashFuncGen* hash = nullptr);
    void set_hash(const HashFuncGen* hash) { hash_ = hash; }
    void add(std::string_view key);
    void add(const std::string& key) { add(std::string_view(key)); }
    double raw() const;
    double estimate() const;
    void reset();
    int b() const { return B_; }
    std::size_t m() const { return static_cast<std::size_t>(1) << B_; }
    std::size_t mem() const;
    std::size_t zeros() const;
private:
    int B_;
    std::size_t m_;
    std::vector<std::uint8_t> M_;
    const HashFuncGen* hash_;
    static double alpha(std::size_t m);
};
