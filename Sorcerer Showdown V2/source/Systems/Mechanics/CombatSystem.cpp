#include "../../../header/Systems/Mechanics/CombatSystem.hpp"
#include "../../../header/Systems/Mechanics/TechniqueSystem.hpp"
#include "../../../header/Systems/Mechanics/DomainSystem.hpp"
#include "../../../header/Systems/Mechanics/NeutralizerSystem.hpp"
#include "../../../header/Systems/Mechanics/SorcerySystem.hpp"
#include "../../../header/Systems/ResourceHandler.hpp"



#include "../../../header/CharacterType/CurseUser.hpp"

#include "../../../header/Utilities/Random.hpp"
#include "../../../header/Battlefield.hpp"
#include "../../../header/Logger.hpp"
#include <cmath>

DamageStruct CombatSystem::ResolveDamage(const Character &c, globalums::DamageType type, double amount) {
    if (const auto& crs = c.CanUseSorcery()){
        if (const auto& tech = crs->Jujutsu().technique){
            if (tech->HasBarrier() && (type != globalums::DamageType::BypassTech && type != globalums::DamageType::BypassAll)){
                return {.attack_blocked = true};
            }
        }
    }
    DamageStruct ds{.negated_damage = amount};
    amount = amount * (0.10 + 0.90 * std::exp(-c.State().durability / 450.0));
    ds.damage = amount;
    ds.negated_damage -= ds.damage; 
    return ds;
}

ToolStruct CombatSystem::ResolveCursedTool(Character &attacker, Character &attacked){
    const auto& current_tool = attacker.Equipment().current_tool;
    const double& damage = current_tool->damage;
    const auto& effect = current_tool->given_effect;
    const auto& msg = attacked.Damage(damage, current_tool->damage_type);
    const bool blocked = msg.attack_blocked;
    if (!blocked){
        attacked.State().status_effects.push_back(*effect);
    }
    return {msg.damage, blocked, &*effect};
}

AttackStruct CombatSystem::ResolveAttacking(Character &attacker, Character &attacked) {
    if (&attacker == &attacked){
        return {-1.0};
    }
    double attack_damage = attacker.State().strength;
    auto attack_type = globalums::DamageType::Normal;
    bool is_blackflash{false};

    if (auto* crs = attacker.CanUseSorcery()) {
        if (crs->Amplification().is_usable && crs->Amplification().is_active){
            attack_type = globalums::DamageType::BypassTech;
        }
        if (get_random<std::uint8_t>(1, 100) <= crs->CursedEnergy().bf_chance){
            attack_damage *= 2.5;
            is_blackflash = true;
            auto& op = crs->Output();
            if (op.status == CurseUserOutput::Status::Boosted){
                op.status = CurseUserOutput::Status::Max;
            }else if (op.status == CurseUserOutput::Status::Regular || op.status == CurseUserOutput::Status::BurntOut){
                op.status = CurseUserOutput::Status::Boosted;
            }
            if (op.normalize_tick > 0) {
                op.normalize_tick--;
            }
        }
    }
    const auto rs = attacked.Damage(attack_damage, attack_type);
    const bool is_critical = rs.damage >= 100.0;
    return {rs.damage, is_critical, is_blackflash};
}

TechniqueStruct CombatSystem::ResolveTechnique(CurseUser& attacker, const TechAbility& chosen_ct, Character& attacked){
    const auto [enough_output, output] = TechniqueSystem::ResolveOutput(chosen_ct , attacker);
    const auto [enough_ce, ce] = TechniqueSystem::ResolveCursedEnergy(attacker, chosen_ct , attacked);

    if (!(enough_output && enough_ce)) {
        return{&chosen_ct,enough_output, enough_ce};
    }

    attacker.CursedEnergy().cursed_energy -= ce;
    attacker.Output().current_output += output;
    attacked.Damage(chosen_ct.damage * SorcerySystem::OutputStatusMultiplier(attacker.Output().status), chosen_ct.damage_type);
    return {&chosen_ct , enough_output, enough_ce};
}

void CombatSystem::ResolveDomain(CurseUser &attacker, Battlefield& bf) {
    const auto& domain = attacker.Jujutsu().domain;

    for (const auto& c : bf.battlefield) {
        if (c.get() == &attacker) continue;
        const bool does_hit = DomainSystem::CalculateActualHit(*domain, *c);
        if (does_hit){
            Log::DomainSurehit(DomainSystem::HandleSureHit(*c, *domain));
        }
        if (const auto& sp = c->CanUseSorcery()){
            if (auto& k = sp->Jujutsu().neutralizer; k && k->is_active){
                NeutralizerSystem::HandleDamage(*k, domain->damage);
                Log::DomainSurehit({domain->damage, &*c, &*domain}, Log::SurehitHit::Neutralizer);
            }
        }
    }
}