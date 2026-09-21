#pragma once
#include "../Structs.hpp"
#include <cstdint>

enum class EffectType : std::uint8_t {
    None,
    Decrease, // bleed effects
    Increase,
    Buff,
    Debuff // temporary one time effects
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
    double saved_value{1.0};
    double effect_amount{1.0};
    int turn_amount{3};
    EffectType effect_type{EffectType::None};
    EffectForType effect_for_type{EffectForType::None};
};