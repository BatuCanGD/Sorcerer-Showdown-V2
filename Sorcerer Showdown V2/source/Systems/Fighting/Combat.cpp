#include "../../../header/Systems/Fighting/Combat.hpp"
#include "../../../header/Systems/Fighting/Tech.hpp"
#include "../../../header/Utilities/Random.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"


double Combat::ResolveDamage(Character &c, globalums::DamageType type, double amount) {
    if ([[maybe_unused]] auto crs = dynamic_cast<CurseUser*>(&c)){
        // add technique stuff and reinforcement checking here!!!
    }
    return amount;
}

AttackStruct Combat::ResolveAttacking(Character &attacker, Character &attacked) {
    double attack_damage = attacker.state.strength;
    auto attack_type = globalums::DamageType::Normal;
    bool is_blackflash{};

    if (auto crs = dynamic_cast<CurseUser*>(&attacker)) {
        if (crs->sorcery.can_use_amplification && crs->sorcery.amplification_is_active){
            attack_type = globalums::DamageType::BypassTech;
        }
        if (get_random<int>(1, 100) <= crs->sorcery.bf_chance){
            attack_damage *= 2.5;
            is_blackflash = true;
        }
    }
    attacked.Damage(attack_damage, attack_type);

    const bool is_critical = attack_damage >= 100.0;
    
    return {attack_damage, is_critical, is_blackflash};
}

TechStruct Combat::ResolveTechnique(CurseUser& attacker, Character& attacked){
    bool enough_output = Tech::ResolveCursedEnergy(attacker, , attacked)

    return {};
}