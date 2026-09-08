#include "../../../header/Systems/Fighting/TechniqueSystem.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"
#include "../../../header/Utilities/Input.hpp"
#include "../../../header/Sorcery/Sorcery.hpp"
#include "../../../header/Sorcery/Technique.hpp"

std::pair<bool, double> TechniqueSystem::ResolveOutput(TechAbility chosen_ct, CurseUser& user) {
    const double efficiency = Sorcery::EfficiencyMultiplier(user.Efficiency());
    const double op = chosen_ct.output * efficiency;

    if (user.Output() + op > user.Output(type::Get::Max)){
        return {false, 0.0};
    }
    return {true, op};
}

std::pair<bool, double> TechniqueSystem::ResolveCursedEnergy(CurseUser& user, TechAbility chosen_ct, Character& target){
    double cursed_energy_consumption = chosen_ct.cost;
    if (user.HasSixEyes()){
        cursed_energy_consumption = Sorcery::ApplySixEyes(cursed_energy_consumption);
    }
    if (user.CursedEnergy() < cursed_energy_consumption){
        return {false, 0.0};
    }
    return {true, cursed_energy_consumption};
}

TechAbility TechniqueSystem::ChooseAbility(Technique& tech) {
    tech.PrintName();
    tech.PrintAbilities();
    return tech.GetAbility(get_input<size_t>());
}