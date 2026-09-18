#pragma once
#include <cstdint>

enum class Sacrifice : std::uint8_t {
    Health,
    CursedTool,
    CursedEnergy,
    OutputPotential,
    Domain,
    Technique
};
enum class Boost : std::uint8_t {
    Health,
    CursedTool,
    CursedEnergy,
    OutputPotential,
    Domain,
    Technique
};

struct BindingVow final {
    double sacrifice_percentage{0.5}; // 0.01..0.99
    Sacrifice sacrifice_type{};
    Boost boost_type{};
};