#include "../../../header/Systems/Fighting/CombatSystem.hpp"
#include "../../../header/Systems/Fighting/TechniqueSystem.hpp"
#include "../../../header/Utilities/Random.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"

#include <cmath>

double CombatSystem::ResolveDamage(Character &c, globalums::DamageType type, double amount) {
    amount = amount * (0.10 + 0.90 * std::exp(-c.state.durability / 450.0));
    if ([[maybe_unused]] auto crs = dynamic_cast<CurseUser*>(&c)){
        // add technique stuff and reinforcement checking here!!!
    }
    return amount;
}

AttackStruct CombatSystem::ResolveAttacking(Character &attacker, Character &attacked) {
    double attack_damage = attacker.State().strength;
    auto attack_type = globalums::DamageType::Normal;
    bool is_blackflash{};

    if (auto crs = dynamic_cast<CurseUser*>(&attacker)) {
        if (crs->Sorcery().can_use_amplification && crs->Sorcery().amplification_is_active){
            attack_type = globalums::DamageType::BypassTech;
        }
        if (get_random<int>(1, 100) <= crs->Sorcery().bf_chance){
            attack_damage *= 2.5;
            is_blackflash = true;
        }
    }
    attacked.Damage(attack_damage, attack_type);

    const bool is_critical = attack_damage >= 100.0;
    
    return {attack_damage, is_critical, is_blackflash};
}

void CombatSystem::ResolveTechnique(CurseUser& attacker, Character& attacked){
    auto tech = attacker.technique();
    TechAbility chosen_ct;

    if (attacker.Control().is_player){
        chosen_ct = TechniqueSystem::ChooseAbility(*attacker.technique());
    }else{
        chosen_ct = tech->GetAbility(get_random<size_t>(0 ,tech->GetAbility() - 1));
    }

    const auto [enough_output, output] = TechniqueSystem::ResolveOutput(chosen_ct , attacker);
    const auto [enough_ce, ce] = TechniqueSystem::ResolveCursedEnergy(attacker, chosen_ct , attacked);

    if (!(enough_output && enough_ce)) return;

    attacker.CursedEnergy(type::Expend::Current, ce);
    attacker.Output(type::Type::Add, output);
    attacked.Damage(chosen_ct.damage);
}