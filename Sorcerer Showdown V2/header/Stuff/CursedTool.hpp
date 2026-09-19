#pragma once
#include "../Enums.hpp"
#include "../Structs.hpp"
#include "StatusEffects.hpp"

struct WeaponType {
    EntityInfo identity;
    double damage{1.0};
    StatusEffect given_effect{};
    globalums::DamageType damage_type{globalums::DamageType::Normal};
};