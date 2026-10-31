#include "../../../header/Systems/Mechanics/TechniqueSystem.hpp"
#include "../../../header/Character.hpp"
#include "../../../header/Logger.hpp"
#include "../../../header/Utilities/Input.hpp"
#include "../../../header/Systems/Mechanics/SorcerySystem.hpp"
#include "../../../header/Sorcery/Technique.hpp"
#include "../../../header/Utilities/Random.hpp"
#include <algorithm>

std::pair<bool, double> TechniqueSystem::ResolveOutput(TechAbility chosen_ct, Character& user) {
    auto& cr = user.curseUser;
    const double op = chosen_ct.output * SorcerySystem::EfficiencyMultiplier(cr->ce_system.efficiency);
    if (cr->output.current_output + op > cr->output.max_output_potential){
        return {false, 0.0};
    }
    return {true, op};
}

std::pair<bool, double> TechniqueSystem::ResolveCursedEnergy(TechAbility chosen_ct, Character& user){
    double cursed_energy_consumption = SorcerySystem::ApplySpendingMultiplier(chosen_ct.cost, user);
    auto& cr = user.curseUser;
    if (cr->ce_system.cursed_energy < cursed_energy_consumption){
        return {false, 0.0};
    }
    return {true, cursed_energy_consumption};
}

TechAbility* TechniqueSystem::GetAbility(Technique &tech, AbilityType type) {
    if (tech.abilities.empty()){
        return nullptr;
    }
    switch(type){
        case TechniqueSystem::AbilityType::Strongest: {
            TechAbility* t{nullptr};
            for (auto& c : tech.abilities) {
                if (!t || c.damage > t->damage) t = &c;
            }
            return t;
        }
        case TechniqueSystem::AbilityType::LeastCE: {
            TechAbility* t{nullptr};
            for (auto& c : tech.abilities) {
                if (!t || c.cost < t->cost) t = &c;
            }
            return t;
        }
        case TechniqueSystem::AbilityType::LeastOutput: {
            TechAbility* t{nullptr};
            for (auto& c : tech.abilities) {
                if (!t || c.output < t->output) t = &c;
            }
            return t;
        }
        case TechniqueSystem::AbilityType::Random:
            return &tech.abilities[get_random<size_t>(0, tech.abilities.size() - 1)];
    }
    return nullptr;
}

double TechniqueSystem::GetBarrierCost(const Technique &tech){
    return 125.0 * (tech.abilities.empty() ? 0.5 : tech.abilities.size());
}

double TechniqueSystem::CalculateDamage(const TechAbility& tc, const Character& c) {
    return tc.damage * (tc.chant_amount > 0 ? 1.0 + tc.chant_amount / 2.0 : 1.0) * SorcerySystem::OutputStatusMultiplier(c.curseUser->output.status);
}

void TechniqueSystem::SetBarrier(Sorcery::CurseUserSystem& cr, bool t){
    if (auto& c = cr.jujutsu.technique) {
        if (!c->barrier.can_use_barrier) {
            return;
        }
        if (cr.ce_system.cursed_energy < GetBarrierCost(*c)) {
            if (!t){
                c->barrier.is_active = t;
            }
            return;
        }
        c->barrier.is_active = t;
    }
}

TechAbility& TechniqueSystem::ChooseAbility(Technique& tech) {
    Log::TechniqueInfo(tech, Log::CTLogType::Abilities);
    size_t i = get_input<size_t>() - 1;
    while(i >= tech.abilities.size()) {
        Log::Error("Invalid Choice");
        i = get_input<size_t>() - 1;
    }
    return tech.abilities[i];
}

bool TechniqueSystem::HasBarrier(const Technique &tech){
    return tech.barrier.can_use_barrier && tech.barrier.is_active;
}

void TechniqueSystem::ChantAbility(TechAbility &tech, const Character& cu){
    tech.chant_amount = std::min(tech.chant_amount + 1, 5);
    Log::TechniqueChant(cu, tech);
}