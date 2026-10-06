#pragma once
#include "../Info.hpp"
#include <cstdint>

enum class SacrificeType : std::uint8_t {
    Health,
    CursedEnergy,
    OutputPotential,
};

struct BindingVow final {
    EntityInfo identity{};
    double percentage{0.5}; // 0.01 -- 0.99
    bool applied{};
    SacrificeType loss{};
    SacrificeType gain{};
};