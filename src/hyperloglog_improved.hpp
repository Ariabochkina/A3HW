#pragma once

#include "hyperloglog.hpp"

class HyperLogLogImproved {
public:
    explicit HyperLogLogImproved(int B, const HashFuncGen* hash = nullptr);
    void set_hash(const HashFuncGen* hash) { hll_.set_hash(hash); }
    void add(std::string_view key) { hll_.add(key); }
    void add(const std::string& key) { hll_.add(key); }
    double estimate() const;
    void reset() { hll_.reset(); }
    int b() const { return hll_.b(); }
    std::size_t m() const { return hll_.m(); }
    std::size_t mem() const { return hll_.mem(); }
private:
    HyperLogLog hll_;
    static double bias(std::size_t m, double raw_est);
};
