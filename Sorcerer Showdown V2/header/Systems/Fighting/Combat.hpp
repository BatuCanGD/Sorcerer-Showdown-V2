#pragma once
#include "../../CharacterType/Character.hpp"
#include "../../Enums.hpp"

struct AttackStruct final {
    double damage;
    bool is_critical;
    bool is_blackflash;
};

struct Combat final // hand 2 hand, no techniques or domains included.
{
    static double ResolveDamage(Character& c, globalums::DamageType type, double amount);
    static AttackStruct ResolveAttacking(Character& attacker, Character& attacked);
};