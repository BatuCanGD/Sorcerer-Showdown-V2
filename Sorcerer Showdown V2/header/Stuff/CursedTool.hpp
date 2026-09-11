#pragma once
#include "../Enums.hpp"
#include "../Structs.hpp"

struct WeaponType {
    EntityInfo identity;
    double damage{1.0};
    globalums::DamageType damage_type{globalums::DamageType::Normal};
};