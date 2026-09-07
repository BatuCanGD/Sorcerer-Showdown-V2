#pragma once
#include "../../Enums.hpp"

class Character;
class CurseUser;

struct AttackStruct final {
    double damage;
    bool is_critical;
    bool is_blackflash;
};

struct TechStruct final {
    double damage;
    bool user_technique_nullified;
    bool target_technique_bypassed;
};

struct Combat final  {
    static double ResolveDamage(Character& c, globalums::DamageType type, double amount);
    static AttackStruct ResolveAttacking(Character& attacker, Character& attacked);
    static TechStruct ResolveTechnique(CurseUser& attacker, Character& attacked);
};