#include "../../header/CharacterUtils/CharacterHelper.hpp"
#include "../../header/Utilities/Random.hpp"
#include "../../header/CharacterType/CurseUser.hpp"


double Helper::DealWithDamage(Character& c, enumtype::DamageType type, double amount){
    if ([[maybe_unused]] auto crs = dynamic_cast<CurseUser*>(&c)){
        // add technique stuff and reinforcement checking here!!!
    }
    return amount;
}
void Helper::DealWithAttacking(Character &attacker, Character &attacked) {
    double attack_damage = attacker.state.strength;
    auto attack_type = enumtype::DamageType::Normal;

    if (auto crs = dynamic_cast<CurseUser*>(&attacker)) {
        if (crs->sorcery.can_use_amplification && crs->sorcery.amplification_is_active){
            attack_type = enumtype::DamageType::BypassTech;
        }
        if (get_random<int>(1, 100) <= crs->sorcery.bf_chance){
            attack_damage *= 2.5;
        }
    }
    attacked.Damage(attack_damage, attack_type);
}