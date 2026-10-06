#include "../../../header/Systems/Mechanics/VowSystem.hpp"
#include "../../../header/CharacterType/CurseUser.hpp"
#include "../../../header/Utilities/Random.hpp"
#include "../../../header/Systems/Stringet.hpp"
#include <stdexcept>
#include <format>

void VowSystem::ApplyVows(CurseUser &c) {
    for (auto& cb : c.Jujutsu().binding_vows) {
        if (cb.applied) continue;
        if (VowSystem::DoVow(c, VowSystem::GetVowCost(c, cb))) {
            cb.applied = true;
        }
    }
}

bool VowSystem::DoVow(CurseUser &c, const VowCostSystem &vcs) {
    switch(vcs.bv->loss) {
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
    switch(vcs.bv->gain){
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
    if (bv.gain == bv.loss){ 
        return {};
    }
    const double sac_mult = bv.percentage;
    
    double sacrifice_amount{};
    switch(bv.loss){
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
    switch (bv.gain) {
        case SacrificeType::CursedEnergy:
            switch (bv.loss) {
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
            switch (bv.loss) {
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
            switch (bv.loss) {
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

const EntityInfo VowSystem::GenerateVowId(const BindingVow& bv) {
    std::string name{}, color{}, desc{};

    const std::uint8_t roll = get_random<std::uint8_t>(1, 10);

    if (roll >= 7){
        color = "\x1b[36m";
    }else if (roll >= 3){
        color = "\x1b[33m";
    }else{
        color = "\x1b[34m";
    }
    const auto l = Stringet::SacrificeStr(bv.loss);
    const auto g = Stringet::SacrificeStr(bv.gain);
    name = l + " for " + g;

    desc = std::format("Sacrifices {:.1f}% of {} for {}", bv.percentage * 100.0, l, g);

    return {name,color , desc};
}
const BindingVow VowSystem::CreateVow(CurseUser& c, const BattleIQ& bq) {
    BindingVow vow{};
    SacrificeType gain{};
    SacrificeType loss{};
    double percentage{};

    switch(bq.fighting_style){
        case FightingStyle::Aggressive:
            loss = c.State().health < c.State().max_health * 0.35 ? 
            SacrificeType::OutputPotential : SacrificeType::Health;
            if (loss == SacrificeType::Health){
                gain = SacrificeType::OutputPotential;
            } else if (loss == SacrificeType::OutputPotential){
                gain = SacrificeType::Health;
            }else{
                gain = SacrificeType::CursedEnergy;
            }
            break;
        case FightingStyle::Defensive:
            loss = c.State().health < c.State().max_health * 0.50 ? 
            SacrificeType::OutputPotential : SacrificeType::CursedEnergy;
            if (loss == SacrificeType::CursedEnergy){
                gain = SacrificeType::OutputPotential;
            } else if (loss == SacrificeType::OutputPotential){
                gain = SacrificeType::Health;
            }else{
                gain = SacrificeType::CursedEnergy;
            }
            break;
        case FightingStyle::Mixed:
            if (get_random<uint8_t>(0, 1) == 1){
                loss = c.State().health < c.State().max_health * get_random<double>(0.01, 0.99) ? 
                SacrificeType::OutputPotential : SacrificeType::Health;
            }else{
                loss = SacrificeType::CursedEnergy;
            }
            if (loss == SacrificeType::CursedEnergy){
                gain = SacrificeType::OutputPotential;
            } else if (loss == SacrificeType::OutputPotential){
                gain = SacrificeType::Health;
            }else{
                gain = SacrificeType::CursedEnergy;
            }
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

    vow.percentage = percentage;
    vow.gain = gain;
    vow.loss = loss;
    vow.identity = VowSystem::GenerateVowId(vow);

    c.Jujutsu().binding_vows.push_back(vow);
    return vow;
}