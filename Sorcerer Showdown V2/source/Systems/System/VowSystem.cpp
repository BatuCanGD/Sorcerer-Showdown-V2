#include "../../../header/Systems/System/VowSystem.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"

#include <stdexcept>

void VowSystem::ApplyVows(CurseUser &c) {
    for (auto& cb : c.Jujutsu().binding_vows) {
        if (cb.applied_vow) continue;
        if (VowSystem::DoVow(c, VowSystem::GetVowCost(c, cb))) {
            cb.applied_vow = true;
        }
    }
}

bool VowSystem::DoVow(CurseUser &c, const VowCostSystem &vcs) {
    switch(vcs.bv->gain_type) {
        case SacrificeType::CursedEnergy:
            c.CursedEnergy().max_cursed_energy -= vcs.sacrifice;
            break;
        case SacrificeType::Health:
            c.State().max_health -= vcs.sacrifice;
            break;
        case SacrificeType::OutputPotential:
            c.Output().max_output_potential -= vcs.sacrifice;
            break;
        default:
            throw std::runtime_error("Unexpected SacrificeType Type");
    }
    switch(vcs.bv->sacrifice_type){
        case SacrificeType::CursedEnergy:
            c.CursedEnergy().max_cursed_energy += vcs.sacrifice;
            break;
        case SacrificeType::Health:
            c.State().max_health += vcs.sacrifice;
            break;
        case SacrificeType::OutputPotential:
            c.Output().max_output_potential += vcs.sacrifice;
            break;
        default:
            throw std::runtime_error("Unexpected SacrificeType Type");
    }
    return true;
}

VowCostSystem VowSystem::GetVowCost(const CurseUser& c, const BindingVow& bv){ // for the stat multiplier
    if (bv.gain_type == bv.sacrifice_type){ 
        return {};
    }
    const double sac_mult = bv.sacrifice_percentage;
    
    double sacrifice_amount{};

    switch(bv.sacrifice_type){
        case SacrificeType::CursedEnergy:
            sacrifice_amount = c.CursedEnergy().max_cursed_energy * sac_mult;
            break;
        case SacrificeType::Health:
            sacrifice_amount = c.State().max_health * sac_mult;
            break;
        case SacrificeType::OutputPotential:
            sacrifice_amount = c.Output().max_output_potential * sac_mult;
            break;
        default:
            break;
    }

    double gain_amount{};

    switch (bv.gain_type) {
        case SacrificeType::CursedEnergy:
            switch (bv.sacrifice_type) {
                case SacrificeType::Health:
                    gain_amount = sacrifice_amount * 20.0;
                    break;

                case SacrificeType::OutputPotential:
                    gain_amount = sacrifice_amount * 50.0;
                    break;
                default:
                    break;
            }
            break;
        case SacrificeType::Health:
            switch (bv.sacrifice_type) {
                case SacrificeType::CursedEnergy:
                    gain_amount = sacrifice_amount * 0.05;
                    break;

                case SacrificeType::OutputPotential:
                    gain_amount = sacrifice_amount * 5.0;
                    break;
                default:
                    break;
            }
            break;
        case SacrificeType::OutputPotential:
            switch (bv.sacrifice_type) {
                case SacrificeType::CursedEnergy:
                    gain_amount = sacrifice_amount * 0.015;
                    break;
                case SacrificeType::Health:
                    gain_amount = sacrifice_amount * 0.20;
                    break;

                default:
                    break;
            }
            break;
        default:
            break;
    }

    return {&bv ,sacrifice_amount, gain_amount};
}