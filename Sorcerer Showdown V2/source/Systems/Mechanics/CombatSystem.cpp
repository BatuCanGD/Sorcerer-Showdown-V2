#include "../../../header/Systems/Mechanics/CombatSystem.hpp"
#include "../../../header/Systems/Mechanics/TechniqueSystem.hpp"
#include "../../../header/Systems/Mechanics/DomainSystem.hpp"
#include "../../../header/Systems/Mechanics/ShikigamiSystem.hpp"
#include "../../../header/Systems/Mechanics/NeutralizerSystem.hpp"
#include "../../../header/Systems/CharacterSystem.hpp"
#include "../../../header/Systems/ResourceHandler.hpp"
#include "../../../header/Utilities/Random.hpp"
#include "../../../header/Battlefield.hpp"
#include "../../../header/Character.hpp"
#include "../../../header/Logger.hpp"
#include <variant>

DamageStruct CombatSystem::ResolveDamage(const std::variant<Character*, Shikigami*> car, DamageType type, double amount) {
    if (std::holds_alternative<Character*>(car)) {
        auto& c = std::get<Character*>(car);
        const auto& crs = c->curseUser;
        if (crs){
            if (const auto& tech = crs->jujutsu.technique){
                if (TechniqueSystem::HasBarrier(*tech) && !DamageCheck::BypassesTech(type)){
                    return {.attack_blocked = true};
                }
            }
        }
        DamageStruct ds{.negated_damage = amount};
        const double reinforcement = crs && (type != DamageType::BypassAll && type != DamageType::BypassRein) ? crs->ce_system.reinforcement_amount / 1.75 : 0.0;
        const double durability = type != DamageType::BypassAll ? c->state.durability : 0.0;
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
    const auto& current_tool = attacker.equipment.current_tool;
    const auto& effect = current_tool->given_effect;
    if (std::holds_alternative<Character*>(attacked)) {
        auto& c = std::get<Character*>(attacked);
        if (&attacker == c) {
            return {nullptr, -1.0};
        }
        const auto msg = CharacterSystem::Damage(*c, current_tool->damage, current_tool->damage_type);
        if (!msg.attack_blocked && effect)  {
            for (const auto& at : c->state.status_effects){
                if (at.id.name == effect->id.name) return {&*current_tool, msg.damage };
            }
            c->state.status_effects.push_back(*effect);
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
    double attack_damage = attacker.state.strength;
    auto attack_type = DamageType::Normal;
    bool is_blackflash{false};

    if (auto& crs = attacker.curseUser) {
        if (crs->amplification.can_use_amp && crs->amplification.is_active){
            attack_type = DamageType::BypassTech;
        }else if (!crs->jujutsu.shikigami.empty()){
            for (const auto& s : crs->jujutsu.shikigami){
                if (s.summon_type != SummonType::Active || s.active_ability != ActiveAbility::TechniqueBypass){
                    continue;
                }
                attack_type = DamageType::BypassTech;
                break;
            }
        }
        if (get_random<std::uint8_t>(1, 100) <= crs->ce_system.bf_chance){
            attack_damage *= 2.5;
            is_blackflash = true;
            auto& op = crs->output;
            if (op.status == Sorcery::OutputSystem::Status::Boosted){
                op.status = Sorcery::OutputSystem::Status::Max;
            }else if (op.status == Sorcery::OutputSystem::Status::Regular || op.status == Sorcery::OutputSystem::Status::BurntOut){
                op.status = Sorcery::OutputSystem::Status::Boosted;
            }
            if (op.normalize_tick > 0) {
                op.normalize_tick--;
            }
        }
    }

    if (is_char){
        auto& c = std::get<Character*>(attacked);
        const auto rs = CharacterSystem::Damage(*c, attack_damage, attack_type);
        const bool is_critical = rs.damage >= c->state.max_health * 0.15;
        return {rs.damage, rs.attack_blocked ,is_critical, is_blackflash};
    }else{
        auto& c = std::get<Shikigami*>(attacked);
        const auto rs = ShikigamiSystem::Damage(*c, attack_damage, attack_type);
        const bool is_critical = rs.damage >= c->stats.max_health * 0.225;
        return {rs.damage, rs.attack_blocked, is_critical, is_blackflash};
    }
}

TechniqueStruct CombatSystem::ResolveTechnique(Character& attacker, TechAbility& chosen_ct, std::variant<Character*, Shikigami*> attacked){
    const auto [enough_output, output] = TechniqueSystem::ResolveOutput(chosen_ct , attacker);
    const auto [enough_ce, ce] = TechniqueSystem::ResolveCursedEnergy(chosen_ct, attacker);
    
    if (!(enough_output && enough_ce)) {
        return{&chosen_ct, &attacker, attacked, 0.0 ,enough_output, enough_ce};
    }
    auto& crs = attacker.curseUser;

    crs->ce_system.cursed_energy -= ce;
    crs->output.current_output += output;

    DamageStruct ds{};
    if (std::holds_alternative<Character*>(attacked)) {
        auto& chr = std::get<Character*>(attacked);
        if (chosen_ct.given_effect) chr->state.status_effects.push_back(*chosen_ct.given_effect);
        ds = CharacterSystem::Damage(*chr, TechniqueSystem::CalculateDamage(chosen_ct, attacker), chosen_ct.damage_type);
    }else {
        ds = ShikigamiSystem::Damage(*std::get<Shikigami*>(attacked), TechniqueSystem::CalculateDamage(chosen_ct, attacker), chosen_ct.damage_type);
    }
    if (chosen_ct.chant_amount > 0) {
        chosen_ct.chant_amount = 0;
    }
    return {&chosen_ct, &attacker, attacked, ds.damage, ds.attack_blocked, enough_output, enough_ce};
}

void CombatSystem::ResolveDomain(Character &attacker, Battlefield& bf) {
    const auto& domain = attacker.curseUser->jujutsu.domain;

    for (const auto& c : bf.battlefield) {
        if (c.get() == &attacker) continue;
        const bool does_hit = DomainSystem::CalculateActualHit(*domain, *c);
        if (does_hit){
            Log::DomainSurehit(DomainSystem::HandleSureHit(*c, *domain));
        }
        if (auto& sp = c->curseUser){
            if (auto& k = sp->jujutsu.neutralizer; k && k->is_active){
                NeutralizerSystem::Damage(*k, domain->damage);
                Log::DomainSurehit({domain->damage, c.get(), &*domain}, Log::SurehitHit::Neutralizer);
            }
        }
    }
}