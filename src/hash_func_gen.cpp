#include "hash_func_gen.hpp"

PolynomialHash32::PolynomialHash32(std::uint32_t base) : base_(base) {}

std::uint32_t PolynomialHash32::operator()(const std::string& key) const {
    return (*this)(std::string_view(key));
}

std::uint32_t PolynomialHash32::operator()(std::string_view key) const {
    std::uint32_t h = 0;
    for (char c : key)
        h = h * base_ + static_cast<std::uint8_t>(c);
    return h;
}

std::uint32_t FNV1aHash32::operator()(const std::string& key) const {
    return (*this)(std::string_view(key));
}

std::uint32_t FNV1aHash32::operator()(std::string_view key) const {
    constexpr std::uint32_t FNV_OFFSET = 2166136261u;
    constexpr std::uint32_t FNV_PRIME = 16777619u;
    std::uint32_t h = FNV_OFFSET;
    for (char c : key) {
        h ^= static_cast<std::uint8_t>(c);
        h *= FNV_PRIME;
    }
    return h;
}
