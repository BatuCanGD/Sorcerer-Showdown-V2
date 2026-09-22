#pragma once
#include "../../Enums.hpp"

class Character;
class CurseUser;
struct Battlefield;
struct StatusEffect;
struct TechStruct;
struct TechAbility;

struct AttackStruct final {
    double damage;
    bool is_critical;
    bool is_blackflash;
};

struct ToolStruct final {
    double damage;
    bool did_hit;
    StatusEffect* applied_effect;
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
    bool did_paralyze;
};

struct CombatSystem final  {
    static DamageStruct ResolveDamage(Character& c, globalums::DamageType type, double amount);
    static ToolStruct ResolveCursedTool(Character& attacker, Character& attacked);
    static AttackStruct ResolveAttacking(Character& attacker, Character& attacked);
    static TechniqueStruct ResolveTechnique(CurseUser& attacker, const TechAbility& tc, Character& attacked);
    static DomainStruct ResolveDomain(CurseUser& attacker, Battlefield& bf);
};