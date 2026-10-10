#include "../../../header/Systems/Mechanics/SorcerySystem.hpp"
#include <cmath>


double SorcerySystem::ApplySpendingMultiplier(double amount, const CurseUser& c) {
    amount *= SorcerySystem::EfficiencyMultiplier(c.CursedEnergy().efficiency);
    if (c.Traits().six_eyes) amount = SorcerySystem::ApplySixEyes(amount);
    return amount;
}

double SorcerySystem::EfficiencyMultiplier(CursedEnergySystem::Efficiency type) noexcept {
    switch(type){
        case CursedEnergySystem::Efficiency::Absolute: return 0.40;
        case CursedEnergySystem::Efficiency::Ultimate: return 0.55;
        case CursedEnergySystem::Efficiency::Extreme:  return 0.65;
        case CursedEnergySystem::Efficiency::Expert:   return 0.80;
        case CursedEnergySystem::Efficiency::Stable:   return 1.00;
        case CursedEnergySystem::Efficiency::Unstable: return 1.30;
        case CursedEnergySystem::Efficiency::Rough:    return 1.75;
        case CursedEnergySystem::Efficiency::Wasteful: return 2.25;
    }
    return 1.0;
}

double SorcerySystem::OutputStatusMultiplier(CurseUserOutput::Status type) noexcept {
    switch(type){
        case CurseUserOutput::Status::BurntOut: return 0.15;
        case CurseUserOutput::Status::Regular:  return 1.0;
        case CurseUserOutput::Status::Boosted:  return 1.45;
        case CurseUserOutput::Status::Max:      return 2.0;
    }
    return 1.0;
    
}
double SorcerySystem::ApplyRCTCost(const double amount) {
    if (amount <= 0.0) {
        return 0.0;
    }
    return amount * (0.70 * std::exp(amount / 100.0));
}
double SorcerySystem::ApplyReinforcementCost(const double amount) {
    if (amount <= 0.0) {
        return 0.0;
    }
    return amount * (0.80 * std::exp(amount / 150.0));
}
double SorcerySystem::ApplySixEyes(const double amount){
    if (amount <= 0.0){
        return 0.0;
    }
    return amount * 0.2;
}

void SorcerySystem::ApplyBurnOut(CurseUser &c){
    c.Output().status = CurseUserOutput::Status::BurntOut;
    c.Output().normalize_tick = 0;
}