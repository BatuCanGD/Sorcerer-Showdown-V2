#pragma once
#include "../../Enums.hpp"

class Character;
class CurseUser;
struct battlefield;
struct TechStruct;

struct AttackStruct final {
    double damage;
    bool is_critical;
    bool is_blackflash;
};

struct DamageStruct final {
    double damage;
    double negated_damage;
    bool attack_blocked;
};

struct TechniqueStruct final {
    bool enough_output;
    bool enough_ce;
};

struct DomainStruct final {
    int hit_amount;
};

struct CombatSystem final  {
    static DamageStruct ResolveDamage(Character& c, globalums::DamageType type, double amount);
    static AttackStruct ResolveAttacking(Character& attacker, Character& attacked);
    static TechniqueStruct ResolveTechnique(CurseUser& attacker, Character& attacked);
    static DomainStruct ResolveDomain(CurseUser& attacker, battlefield& bf);
};