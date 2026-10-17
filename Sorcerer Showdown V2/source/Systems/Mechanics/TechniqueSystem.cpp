#include "../../../header/Systems/Mechanics/TechniqueSystem.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"
#include "../../../header/Logger.hpp"
#include "../../../header/Utilities/Input.hpp"
#include "../../../header/Systems/Mechanics/SorcerySystem.hpp"
#include "../../../header/Sorcery/Technique.hpp"
#include "../../../header/Utilities/Random.hpp"

std::pair<bool, double> TechniqueSystem::ResolveOutput(TechAbility chosen_ct, CurseUser& user) {
    const double op = chosen_ct.output * SorcerySystem::EfficiencyMultiplier(user.CursedEnergy().efficiency);
    if (user.Output().current_output + op > user.Output().max_output_potential){
        return {false, 0.0};
    }
    return {true, op};
}

std::pair<bool, double> TechniqueSystem::ResolveCursedEnergy(CurseUser& user, TechAbility chosen_ct, Character& target){
    double cursed_energy_consumption = SorcerySystem::ApplySpendingMultiplier(chosen_ct.cost, user);
    if (user.CursedEnergy().cursed_energy < cursed_energy_consumption){
        return {false, 0.0};
    }
    return {true, cursed_energy_consumption};
}

TechAbility* TechniqueSystem::GetAbility(Technique &tech, AbilityType type) {
    if (tech.abilities.empty()){
        throw std::runtime_error("How can a technique not have atleast 1 ability");
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
    throw std::runtime_error("couldnt find any fitting technique ability");
}

double TechniqueSystem::CalculateDamage(const TechAbility& tc, const CurseUser& c) {
    return tc.damage * (tc.chant_amount > 0 ? 1.0 + tc.chant_amount / 2.0 : 1.0) * SorcerySystem::OutputStatusMultiplier(c.Output().status);
}

TechAbility& TechniqueSystem::ChooseAbility(Technique& tech) {
    Log::TechniqueInfo(tech, Log::CTLogType::Both, Log::LogDetailType::Detailed);
    size_t i = get_input<size_t>() - 1;
    while(i >= tech.abilities.size()) {
        Log::Error();
        i = get_input<size_t>() - 1;
    }
    return tech.abilities[i];
}

bool TechniqueSystem::HasBarrier(const Technique &tech){
    return tech.barrier.can_use_barrier && tech.barrier.is_active;
}

void TechniqueSystem::ChantAbility(TechAbility &tech, const CurseUser& cu){
    tech.chant_amount++;
    Log::TechniqueChant(cu, tech);
}