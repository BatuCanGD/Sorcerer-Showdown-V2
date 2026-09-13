#include "../../../header/Systems/System/CombatSystem.hpp"
#include "../../../header/Systems/System/TechniqueSystem.hpp"
#include "../../../header/Systems/ResourceHandler.hpp"
#include "../../../header/Helper/DomainHelper.hpp"
#include "../../../header/Battlefield.hpp"
#include "../../../header/Utilities/Random.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"

#include <cmath>

DamageStruct CombatSystem::ResolveDamage(Character &c, globalums::DamageType type, double amount) {
    DamageStruct ds{};
    ds.negated_damage = amount;
    amount = amount * (0.10 + 0.90 * std::exp(-c.state.durability / 450.0));
    if (auto crs = c.CanUseSorcery()){
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

AttackStruct CombatSystem::ResolveAttacking(Character &attacker, Character &attacked) {
    double attack_damage = attacker.State().strength;
    auto attack_type = globalums::DamageType::Normal;
    bool is_blackflash{false};

    if (auto crs = dynamic_cast<CurseUser*>(&attacker)) {
        if (crs->Amplification().is_usable && crs->Amplification().is_active){
            attack_type = globalums::DamageType::BypassTech;
        }
        if (get_random<int>(1, 100) <= crs->CursedEnergySys().bf_chance){
            attack_damage *= 2.5;
            is_blackflash = true;
        }
    }
    attacked.Damage(attack_damage, attack_type);
    const bool is_critical = attack_damage >= 100.0;
    return {attack_damage, is_critical, is_blackflash};
}

TechniqueStruct CombatSystem::ResolveTechnique(CurseUser& attacker, Character& attacked){
    auto& tech = attacker.Jujutsu().technique;
    TechAbility chosen_ct;

    if (attacker.Control().is_player){
        chosen_ct = TechniqueSystem::ChooseAbility(tech.value());
    }else{
        chosen_ct = tech->GetAbility(get_random<size_t>(0 ,tech->GetAbility() - 1));
    }

    const auto [enough_output, output] = TechniqueSystem::ResolveOutput(chosen_ct , attacker);
    const auto [enough_ce, ce] = TechniqueSystem::ResolveCursedEnergy(attacker, chosen_ct , attacked);

    if (!(enough_output && enough_ce)) {
        return{enough_output, enough_ce };
    }

    attacker.CursedEnergy(OpType::Expend, ce);
    attacker.Output().current_output += output;
    attacked.Damage(chosen_ct.damage);
    return {enough_output, enough_ce};
}

DomainStruct CombatSystem::ResolveDomain(CurseUser &attacker, battlefield& bf) {
    DomainStruct dst{};
    ResourceHandler::UsedDomain(attacker);
    const auto& domain = attacker.Jujutsu().domain;
    const bool does_paralyze = domain->surehit_type == SurehitType::Paralyzing;

    for (const auto& c : bf.battlefield) {
        const auto& [can_hit, damage] = DomainHelper::CalculateHit(domain, c);
        if (can_hit){
            c->Health(OpType::Expend, damage);
            if (does_paralyze) {
                c->State().is_stunned = true;
            }
            dst.hit_amount++;
        }
    }

    return dst;
}