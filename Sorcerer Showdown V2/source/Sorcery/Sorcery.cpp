#include "../../header/Sorcery/Sorcery.hpp"


double Sorcery::EfficiencyMultiplier(CurseUserSystem::Efficiency type) noexcept {
    switch(type){
        case CurseUserSystem::Efficiency::Absolute: return 0.40;
        case CurseUserSystem::Efficiency::Ultimate: return 0.55;
        case CurseUserSystem::Efficiency::Extreme:  return 0.65;
        case CurseUserSystem::Efficiency::Expert:   return 0.80;
        case CurseUserSystem::Efficiency::Stable:   return 1.00;
        case CurseUserSystem::Efficiency::Unstable: return 1.30;
        case CurseUserSystem::Efficiency::Rough:    return 1.75;
        case CurseUserSystem::Efficiency::Wasteful: return 2.25;
    }
    return 1.0;
}

double Sorcery::ApplySixEyes(double amount){
    return amount * 0.25;
}