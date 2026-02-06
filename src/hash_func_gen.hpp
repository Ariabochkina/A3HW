#pragma once

#include <cstdint>
#include <string>
#include <string_view>

class HashFuncGen {
public:
    virtual ~HashFuncGen() = default;
    virtual std::uint32_t operator()(const std::string& key) const = 0;
    virtual std::uint32_t operator()(std::string_view key) const = 0;
};

class PolynomialHash32 : public HashFuncGen {
public:
    explicit PolynomialHash32(std::uint32_t base = 31u);
    std::uint32_t operator()(const std::string& key) const override;
    std::uint32_t operator()(std::string_view key) const override;
private:
    std::uint32_t base_;
};

class FNV1aHash32 : public HashFuncGen {
public:
    std::uint32_t operator()(const std::string& key) const override;
    std::uint32_t operator()(std::string_view key) const override;
};
