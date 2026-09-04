#pragma once
#include "CharacterType/Character.hpp"

struct AttackStruct final {
    double damage;
    bool is_critical;
    bool is_blackflash;
};

struct CombatHelper final {
    static double DealWithDamage(Character& c, enumtype::DamageType type, double amount);
    static AttackStruct DealWithAttacking(Character& attacker, Character& attacked);
};