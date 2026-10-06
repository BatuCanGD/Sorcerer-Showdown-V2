#pragma once
#include "../Enums.hpp"
#include "../Info.hpp"
#include "StatusEffects.hpp"

struct CursedTool {
    EntityInfo identity;
    double damage{1.0};
    StatusEffect given_effect{};
    globalums::DamageType damage_type{globalums::DamageType::Normal};
};