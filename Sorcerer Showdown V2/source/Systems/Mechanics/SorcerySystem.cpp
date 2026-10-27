#include "../../../header/Systems/Mechanics/SorcerySystem.hpp"
#include "../../../header/Character.hpp"
#include <cmath>


double SorcerySystem::ApplySpendingMultiplier(double amount, const Character& c) {
    auto& cr = c.curseUser;
    amount *= SorcerySystem::EfficiencyMultiplier(cr->ce_system.efficiency);
    if (cr->traits.six_eyes) amount = SorcerySystem::ApplySixEyes(amount);
    return amount;
}

double SorcerySystem::EfficiencyMultiplier(Sorcery::CursedEnergySystem::Efficiency type) noexcept {
    switch(type){
        case Sorcery::CursedEnergySystem::Efficiency::Peak:     return 0.40;
        case Sorcery::CursedEnergySystem::Efficiency::Extreme:  return 0.55;
        case Sorcery::CursedEnergySystem::Efficiency::Expert:   return 0.70;
        case Sorcery::CursedEnergySystem::Efficiency::Precise:  return 0.80;
        case Sorcery::CursedEnergySystem::Efficiency::Stable:   return 1.00;
        case Sorcery::CursedEnergySystem::Efficiency::Unstable: return 1.30;
        case Sorcery::CursedEnergySystem::Efficiency::Rough:    return 1.75;
        case Sorcery::CursedEnergySystem::Efficiency::Wasteful: return 2.25;
    }
    return 1.0;
}

double SorcerySystem::OutputStatusMultiplier(Sorcery::OutputSystem::Status type) noexcept {
    switch(type){
        case Sorcery::OutputSystem::Status::BurntOut: return 0.15;
        case Sorcery::OutputSystem::Status::Regular:  return 1.0;
        case Sorcery::OutputSystem::Status::Boosted:  return 1.45;
        case Sorcery::OutputSystem::Status::Max:      return 2.0;
    }
    return 1.0;
    
}
double SorcerySystem::ApplyRCTCost(const double amount) {
    if (amount <= 0.0) {
        return 0.0;
    }
    return amount * (0.60 * std::exp(amount / 70.0));
}
double SorcerySystem::ApplyReinforcementCost(const double amount) {
    if (amount <= 0.0) {
        return 0.0;
    }
    return amount * (0.80 * std::exp(amount / 110.0));
}
double SorcerySystem::ApplySixEyes(const double amount){
    if (amount <= 0.0){
        return 0.0;
    }
    return amount * 0.05;
}

bool SorcerySystem::CEMoreThanMax(Character &c, double p){
    return c.curseUser->ce_system.cursed_energy >= c.curseUser->ce_system.max_cursed_energy * p;
}

void SorcerySystem::ApplyBurnOut(Character &c){
    c.curseUser->output.status = Sorcery::OutputSystem::Status::BurntOut;
    c.curseUser->output.normalize_tick = 0;
}