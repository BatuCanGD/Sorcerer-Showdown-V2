#pragma once
#include <cstdint>

enum class SacrificeType : std::uint8_t {
    Health,
    CursedEnergy,
    OutputPotential,
};

struct BindingVow final {
    double sacrifice_percentage{0.5}; // 0.01 -- 0.99
    bool applied_vow{};
    SacrificeType sacrifice_type{};
    SacrificeType gain_type{};
};