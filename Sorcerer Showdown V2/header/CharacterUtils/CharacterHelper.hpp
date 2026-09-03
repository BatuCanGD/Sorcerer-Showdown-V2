#pragma once
#include "../CharacterType/Character.hpp"

struct Helper {
    static double DealWithDamage(Character& c, enumtype::DamageType type, double amount);
    static void DealWithAttacking(Character& attacker, Character& attacked);
};