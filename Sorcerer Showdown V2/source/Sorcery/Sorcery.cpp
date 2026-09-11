#include "../../header/Sorcery/Sorcery.hpp"


double Sorcery::EfficiencyMultiplier(CursedEnergySystem::Efficiency type) noexcept {
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

double Sorcery::ApplySixEyes(double amount){
    return amount * 0.25;
}