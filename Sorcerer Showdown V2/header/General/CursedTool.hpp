#pragma once
#include "../DamageType.hpp"
#include "../Info.hpp"
#include "StatusEffects.hpp"
#include <optional>
struct CursedTool final {
    EntityInfo identity;
    double damage{1.0};
    std::optional<StatusEffect> given_effect;
    DamageType damage_type{DamageType::Normal};
};