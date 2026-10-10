#pragma once
#include "../Enums.hpp"
#include "../Info.hpp"
#include "StatusEffects.hpp"
#include <optional>
struct CursedTool final {
    EntityInfo identity;
    double damage{1.0};
    std::optional<StatusEffect> given_effect;
    globalums::DamageType damage_type{globalums::DamageType::Normal};
};