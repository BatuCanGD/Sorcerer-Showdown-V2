#include "../../../header/Systems/System/VowSystem.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"
#include "../../../header/Utilities/Random.hpp"

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
            if (c.CursedEnergy().cursed_energy > c.CursedEnergy().max_cursed_energy) c.CursedEnergy().cursed_energy = c.CursedEnergy().max_cursed_energy;
            break;
        case SacrificeType::Health:
            c.State().max_health -= vcs.sacrifice;
            if (c.State().health > c.State().max_health) c.State().health = c.State().max_health;
            break;
        case SacrificeType::OutputPotential:
            c.Output().max_output_potential -= vcs.sacrifice;
            break;
        default:
            throw std::runtime_error("Unexpected SacrificeType Type");
    }
    switch(vcs.bv->sacrifice_type){
        case SacrificeType::CursedEnergy:
            c.CursedEnergy().max_cursed_energy += vcs.gain;
            break;
        case SacrificeType::Health:
            c.State().max_health += vcs.gain;
            break;
        case SacrificeType::OutputPotential:
            c.Output().max_output_potential += vcs.gain;
            break;
        default:
            throw std::runtime_error("Unexpected SacrificeType Type");
    }
    return true;
}

const VowCostSystem VowSystem::GetVowCost(const CurseUser& c, const BindingVow& bv){ // for the stat multiplier
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

const EntityInfo VowSystem::GetRandomId(const BindingVow& bv) {
    std::string name{}, color{};

    if (bv.sacrifice_percentage >= 0.70){
        color = "\x1b[36m";
    }else if (bv.sacrifice_percentage >= 35){
        color = "\x1b[33m";
    }else{
        color = "\x1b[34m";
    }

    

    return {color, name};
}
const BindingVow VowSystem::CreateVow(CurseUser& c, const BattleIQ& bq) {
    BindingVow vow{};

    SacrificeType gain{};
    SacrificeType loss{};
    double percentage{};

    switch(bq.fighting_style){
        case FightingStyle::Aggressive:
            if (get_random<uint8_t>(0, 1) == 1){
                loss = c.State().health < c.State().max_health * 0.20 ? SacrificeType::OutputPotential : SacrificeType::Health;
            }else{
                loss = SacrificeType::CursedEnergy;
            }
            break;
        case FightingStyle::Defensive:

            break;
        case FightingStyle::Mixed:

            break;
    }

    switch(bq.resource_usage){
        case ResourceUsage::AllOut:
            percentage = get_random<double>(0.75, 0.99);
            break;
        case ResourceUsage::Conservative:
            percentage = get_random<double>(0.01, 0.25);
            break;
        case ResourceUsage::Mixed:
            percentage = get_random<double>(0.25, 0.75);
            break;
    }

    vow.sacrifice_percentage = percentage;
    vow.gain_type = gain;
    vow.sacrifice_type = loss;

    VowSystem::GetRandomId(vow);
    c.Jujutsu().binding_vows.push_back(vow);
    return vow;
}