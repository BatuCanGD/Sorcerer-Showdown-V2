#pragma once
#include "../../DamageType.hpp"
#include <variant>

struct Character;

struct Shikigami;
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
    const std::variant<Character*, Shikigami*> target;
    double damage;
    bool blocked;
    bool enough_output;
    bool enough_ce;
};

namespace CombatSystem {
    DamageStruct ResolveDamage(const std::variant<Character*, Shikigami*> c, DamageType type, double amount);
    AttackStruct ResolveAttacking(Character& attacker, std::variant<Character*, Shikigami*> attacked);
    ToolStruct ResolveCursedTool(Character& attacker, std::variant<Character*, Shikigami*> attacked);
    TechniqueStruct ResolveTechnique(Character& attacker, TechAbility& tc, std::variant<Character*, Shikigami*> attacked);
    void ResolveDomain(Character& attacker, Battlefield& bf);
};