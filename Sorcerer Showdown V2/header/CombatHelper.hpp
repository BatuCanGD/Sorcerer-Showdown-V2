#pragma once
#include "CharacterType/Character.hpp"
#include "Enums.hpp"

struct AttackStruct final {
    double damage;
    bool is_critical;
    bool is_blackflash;
};

struct CombatHelper final {
    static double DealWithDamage(Character& c, globalums::DamageType type, double amount);
    static AttackStruct DealWithAttacking(Character& attacker, Character& attacked);
};