#pragma once
#include "../Structs.hpp"
#include <cstdint>

enum class EffectType : std::uint8_t {
    None,
    Drain, // bleed effects
    Increase,
    Buff,
    Debuff // temporary debuff
};

enum class EffectForType : std::uint8_t {
    None,
    Health,
    Durability,
    Strength,
    CursedEnergy,
    MaxOutput
};


struct StatusEffect final {
    EntityInfo id;
    double effect_amount{1.0};
    int turn_amount{3};
    EffectType effect_type{EffectType::None};
    EffectForType effect_for_type{EffectForType::None};
};