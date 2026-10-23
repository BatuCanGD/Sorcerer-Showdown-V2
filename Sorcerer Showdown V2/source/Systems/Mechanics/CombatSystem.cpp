#include "../../../header/Systems/Mechanics/CombatSystem.hpp"
#include "../../../header/Systems/Mechanics/TechniqueSystem.hpp"
#include "../../../header/Systems/Mechanics/DomainSystem.hpp"
#include "../../../header/Systems/Mechanics/ShikigamiSystem.hpp"
#include "../../../header/Systems/Mechanics/NeutralizerSystem.hpp"
#include "../../../header/Systems/GenericSystems.hpp"
#include "../../../header/Systems/ResourceHandler.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"
#include "../../../header/Utilities/Random.hpp"
#include "../../../header/Battlefield.hpp"
#include "../../../header/Logger.hpp"
#include <variant>

DamageStruct CombatSystem::ResolveDamage(const std::variant<Character*, Shikigami*> car, globalums::DamageType type, double amount) {
    if (std::holds_alternative<Character*>(car)) {
        auto& c = std::get<Character*>(car);
        const auto& crs = c->CanUseSorcery();
        if (crs){
            if (const auto& tech = crs->Jujutsu().technique){
                if (TechniqueSystem::HasBarrier(*tech) && !GenericSystems::BypassesTech(type)){
                    return {.attack_blocked = true};
                }
            }
        }
        DamageStruct ds{.negated_damage = amount};
        const double reinforcement = crs && (type != globalums::DamageType::BypassAll && type != globalums::DamageType::BypassRein) ? crs->CursedEnergy().reinforcement_amount / 1.75 : 0.0;
        const double durability = type != globalums::DamageType::BypassAll ? c->State().durability : 0.0;
        amount = amount * (0.10 + 0.90 * std::exp(-(durability + reinforcement) / 450.0));
        ds.damage = amount;
        ds.negated_damage -= ds.damage; 
        return ds;
    }else{
        auto& c = std::get<Shikigami*>(car);
        DamageStruct ds{.negated_damage = amount};
        amount = amount * (0.10 + 0.90 * std::exp(-(c->stats.durability) / 250.0));
        ds.damage = amount;
        ds.negated_damage -= ds.damage;
        return ds;
    }
}

ToolStruct CombatSystem::ResolveCursedTool(Character &attacker, std::variant<Character*, Shikigami*> attacked){
    const auto& current_tool = attacker.Equipment().current_tool;
    const auto& effect = current_tool->given_effect;
    if (std::holds_alternative<Character*>(attacked)) {
        auto& c = std::get<Character*>(attacked);
        if (&attacker == c) {
            return {nullptr, -1.0};
        }
        const auto msg = c->Damage(current_tool->damage, current_tool->damage_type);
        if (!msg.attack_blocked && effect)  {
            for (const auto& at : c->State().status_effects){
                if (at.id.name == effect->id.name) return {&*current_tool, msg.damage };
            }
            c->State().status_effects.push_back(*effect);
        }
        return {&*current_tool,msg.damage};
    }else {
        auto& c = std::get<Shikigami*>(attacked);
        const auto msg = ShikigamiSystem::Damage(*c, current_tool->damage, current_tool->damage_type);
        return {&*current_tool, msg.damage};
    }
}

AttackStruct CombatSystem::ResolveAttacking(Character &attacker, std::variant<Character*, Shikigami*> attacked) {
    const bool is_char = std::holds_alternative<Character*>(attacked);

    if (is_char) {
        if (&attacker == std::get<Character*>(attacked)){
            return {-1.0};
        }
    }
    double attack_damage = attacker.State().strength;
    auto attack_type = globalums::DamageType::Normal;
    bool is_blackflash{false};

    if (auto* crs = attacker.CanUseSorcery()) {
        if (crs->Amplification().can_use_amp && crs->Amplification().is_active){
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

    if (is_char){
        auto& c = std::get<Character*>(attacked);
        const auto rs = c->Damage(attack_damage, attack_type);
        const bool is_critical = rs.damage >= c->State().max_health * 0.15;
        return {rs.damage, rs.attack_blocked ,is_critical, is_blackflash};
    }else{
        auto& c = std::get<Shikigami*>(attacked);
        const auto rs = ShikigamiSystem::Damage(*c, attack_damage, attack_type);
        const bool is_critical = rs.damage >= c->stats.max_health * 0.225;
        return {rs.damage, rs.attack_blocked, is_critical, is_blackflash};
    }
}

TechniqueStruct CombatSystem::ResolveTechnique(CurseUser& attacker, TechAbility& chosen_ct, std::variant<Character*, Shikigami*> attacked){
    const auto [enough_output, output] = TechniqueSystem::ResolveOutput(chosen_ct , attacker);
    const auto [enough_ce, ce] = TechniqueSystem::ResolveCursedEnergy(chosen_ct, attacker);
    
    if (!(enough_output && enough_ce)) {
        return{&chosen_ct, &attacker, attacked, 0.0 ,enough_output, enough_ce};
    }

    attacker.CursedEnergy().cursed_energy -= ce;
    attacker.Output().current_output += output;

    DamageStruct ds{};
    if (std::holds_alternative<Character*>(attacked)) {
        auto& chr = std::get<Character*>(attacked);
        if (chosen_ct.given_effect) chr->State().status_effects.push_back(*chosen_ct.given_effect);
        ds = chr->Damage(TechniqueSystem::CalculateDamage(chosen_ct, attacker), chosen_ct.damage_type);
    }else {
        ds = ShikigamiSystem::Damage(*std::get<Shikigami*>(attacked), TechniqueSystem::CalculateDamage(chosen_ct, attacker), chosen_ct.damage_type);
    }
    if (chosen_ct.chant_amount > 0) {
        chosen_ct.chant_amount = 0;
    }
    return {&chosen_ct, &attacker, attacked, ds.damage, ds.attack_blocked, enough_output, enough_ce};
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
                NeutralizerSystem::Damage(*k, domain->damage);
                Log::DomainSurehit({domain->damage, &*c, &*domain}, Log::SurehitHit::Neutralizer);
            }
        }
    }
}