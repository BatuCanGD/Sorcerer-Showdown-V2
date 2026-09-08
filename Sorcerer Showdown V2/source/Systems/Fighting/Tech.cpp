#include "../../../header/Systems/Fighting/Tech.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"
#include "../../../header/Sorcery/Sorcery.hpp"
#include "../../../header/Sorcery/Technique.hpp"

bool Tech::ResolveOutput(TechAbility chosen_ct, CurseUser& user) {
    const double efficiency = Sorcery::EfficiencyMultiplier(user.Efficiency());
    const double op = chosen_ct.output * efficiency;

    if (user.Output() + op > user.Output(type::Get::Max)){
        return false;
    }
    return true;
}

bool Tech::ResolveCursedEnergy(CurseUser& user, TechAbility chosen_ct, Character& target){
    double cursed_energy_consumption = chosen_ct.cost;
    if (user.HasSixEyes()){
        cursed_energy_consumption = Sorcery::ApplySixEyes(cursed_energy_consumption);
    }
    if (user.CursedEnergy() < cursed_energy_consumption){
        return false;
    }
    return true;
}