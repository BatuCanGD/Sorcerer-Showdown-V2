#include "../../../header/Systems/System/CombatSystem.hpp"
#include "../../../header/Systems/System/TechniqueSystem.hpp"
#include "../../../header/Systems/System/DomainSystem.hpp"
#include "../../../header/Systems/System/NeutralizerSystem.hpp"
#include "../../../header/Systems/ResourceHandler.hpp"
#include "../../../header/Battlefield.hpp"
#include "../../../header/Utilities/Random.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"

#include <cmath>

DamageStruct CombatSystem::ResolveDamage(const Character &c, globalums::DamageType type, double amount) {
    DamageStruct ds{};
    ds.negated_damage = amount;
    amount = amount * (0.10 + 0.90 * std::exp(-c.State().durability / 450.0));
    if (const auto& crs = c.CanUseSorcery()){
        if (const auto& tech = crs->Jujutsu().technique){
            if (tech->HasBarrier() && (type != globalums::DamageType::BypassTech && type != globalums::DamageType::BypassAll)){
                ds.attack_blocked = true;
            }
        }
    }
    ds.damage = amount;
    ds.negated_damage = ds.negated_damage - ds.damage; 
    return ds;
}

ToolStruct CombatSystem::ResolveCursedTool(Character &attacker, Character &attacked){
    [[maybe_unused]] const auto& current_tool = attacker.Equipment().current_tool;
    const double& damage = current_tool->damage;
    const auto& effect = current_tool->given_effect;
    const auto& msg = attacked.Damage(damage, current_tool->damage_type);
    const bool blocked = msg.attack_blocked;
    if (!blocked){
        attacked.State().status_effects.push_back(effect);
    }
    return {damage, blocked, &effect};
}

AttackStruct CombatSystem::ResolveAttacking(const Character &attacker, Character &attacked) {
    if (&attacker == &attacked){
        return {-1.0};
    }
    double attack_damage = attacker.State().strength;
    auto attack_type = globalums::DamageType::Normal;
    bool is_blackflash{false};

    if (const auto& crs = attacker.CanUseSorcery()) {
        if (crs->Amplification().is_usable && crs->Amplification().is_active){
            attack_type = globalums::DamageType::BypassTech;
        }
        if (get_random<int>(1, 100) <= crs->CursedEnergy().bf_chance){
            attack_damage *= 2.5;
            is_blackflash = true;
        }
    }
    attacked.Damage(attack_damage, attack_type);
    const bool is_critical = attack_damage >= 100.0;
    return {attack_damage, is_critical, is_blackflash};
}

TechniqueStruct CombatSystem::ResolveTechnique(CurseUser& attacker, const TechAbility& chosen_ct, Character& attacked){
    const auto [enough_output, output] = TechniqueSystem::ResolveOutput(chosen_ct , attacker);
    const auto [enough_ce, ce] = TechniqueSystem::ResolveCursedEnergy(attacker, chosen_ct , attacked);

    if (!(enough_output && enough_ce)) {
        return{nullptr,enough_output, enough_ce};
    }

    attacker.CursedEnergy().cursed_energy -= ce;
    attacker.Output().current_output += output;
    attacked.Damage(chosen_ct.damage, chosen_ct.damage_type);
    return {&chosen_ct , enough_output, enough_ce};
}

DomainStruct CombatSystem::ResolveDomain(CurseUser &attacker, Battlefield& bf) {
    const auto& domain = attacker.Jujutsu().domain;
    const auto& damage_type = domain->damage_type;
    const bool does_paralyze = domain->surehit_type == SurehitType::Paralyzing;

    int hit_amount = 0;
    for (const auto& c : bf.battlefield) {
        if (c.get() == &attacker) continue;
        const auto [damage, does_hit] = DomainSystem::CalculateHit(domain, c);
        if (does_hit){
            hit_amount++;
            DomainSystem::HandleSureHit(*c, damage, does_paralyze, damage_type);
        }else if (const auto& sp = c->CanUseSorcery()){
            if (auto& k =sp->Jujutsu().neutralizer){
                NeutralizerSystem::HandleDamage(*k, damage);
            }
        }
    }
    return {hit_amount};
}