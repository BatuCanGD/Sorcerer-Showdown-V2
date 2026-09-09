#pragma once
#include "../../Enums.hpp"

class Character;
class CurseUser;
struct TechStruct;

struct AttackStruct final {
    double damage;
    bool is_critical;
    bool is_blackflash;
};

struct DamageStruct final {
    double damage;
    bool attack_blocked;
};

struct CombatSystem final  {
    static DamageStruct ResolveDamage(Character& c, globalums::DamageType type, double amount);
    static AttackStruct ResolveAttacking(Character& attacker, Character& attacked);
    static void ResolveTechnique(CurseUser& attacker, Character& attacked);
};