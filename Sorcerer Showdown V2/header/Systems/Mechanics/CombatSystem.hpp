#pragma once
#include "../../Enums.hpp"

class Character;
class CurseUser;
struct Battlefield;
struct StatusEffect;
struct CursedTool;
struct TechStruct;
struct TechAbility;

struct AttackStruct final {
    double damage;
    bool is_critical;
    bool is_blackflash;
};

struct ToolStruct final {
    const CursedTool* used_tool;
    double dealt_damage;
};

struct DamageStruct final {
    double damage;
    double negated_damage;
    bool attack_blocked;
};

struct TechniqueStruct final {
    const TechAbility* used_ability;
    bool enough_output;
    bool enough_ce;
};

struct CombatSystem final  {
    static DamageStruct ResolveDamage(const Character& c, globalums::DamageType type, double amount);
    static AttackStruct ResolveAttacking(Character& attacker, Character& attacked);
    static ToolStruct ResolveCursedTool(Character& attacker, Character& attacked);
    static TechniqueStruct ResolveTechnique(CurseUser& attacker, const TechAbility& tc, Character& attacked);
    static void ResolveDomain(CurseUser& attacker, Battlefield& bf);
};