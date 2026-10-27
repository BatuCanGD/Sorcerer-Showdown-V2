#include "../../header/Systems/CharacterSystem.hpp"
#include "../../header/Systems/Mechanics/CombatSystem.hpp"
#include "../../header/Character.hpp"

DamageStruct CharacterSystem::Damage(Character& damaged, double amount, DamageType type){
    DamageStruct ds{.attack_blocked = true};
    if (damaged.state.is_invulnerable){
        return ds;
    }
    ds = CombatSystem::ResolveDamage(&damaged, type, amount);
    if (ds.attack_blocked){
        return ds;
    }
    damaged.state.health -= ds.damage;
    return ds;
}
std::variant<AttackStruct, ToolStruct> CharacterSystem::Attack(Character& c, std::variant<Character*, Shikigami*> cc){
    if (c.equipment.current_tool){
        return CombatSystem::ResolveCursedTool(c, cc);
    }
    return CombatSystem::ResolveAttacking(c, cc);
}
bool CharacterSystem::HPMoreThanMax(const Character& c, double d) {
    return c.state.health >= c.state.max_health * d;
}
std::unique_ptr<Character> CharacterSystem::Clone(const Character &c){
    return std::make_unique<Character>(c);
}