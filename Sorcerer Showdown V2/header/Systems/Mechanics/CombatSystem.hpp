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
    bool blocked;
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
    const Character* user;
    const Character* target;
    double damage;
    bool blocked;
    bool enough_output;
    bool enough_ce;
};

namespace CombatSystem {
    DamageStruct ResolveDamage(const Character& c, globalums::DamageType type, double amount);
    AttackStruct ResolveAttacking(Character& attacker, Character& attacked);
    ToolStruct ResolveCursedTool(Character& attacker, Character& attacked);
    TechniqueStruct ResolveTechnique(CurseUser& attacker, TechAbility& tc, Character& attacked);
    void ResolveDomain(CurseUser& attacker, Battlefield& bf);
};