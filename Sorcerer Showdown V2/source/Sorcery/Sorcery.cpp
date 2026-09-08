#include "../../header/Sorcery/Sorcery.hpp"


double Sorcery::EfficiencyMultiplier(CurseUser::CEfficiency type) noexcept {
    switch(type){
        case CurseUser::CEfficiency::Absolute: return 0.40;
        case CurseUser::CEfficiency::Ultimate: return 0.55;
        case CurseUser::CEfficiency::Extreme:  return 0.65;
        case CurseUser::CEfficiency::Expert:   return 0.80;
        case CurseUser::CEfficiency::Stable:   return 1.00;
        case CurseUser::CEfficiency::Unstable: return 1.30;
        case CurseUser::CEfficiency::Rough:    return 1.75;
        case CurseUser::CEfficiency::Wasteful: return 2.25;
    }
    return 1.0;
}

double Sorcery::ApplySixEyes(double amount){
    return amount * 0.3;
}