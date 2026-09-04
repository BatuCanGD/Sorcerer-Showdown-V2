#include "../../header/CharacterType/CurseUser.hpp"


double CurseUser::CursedEnergy(enumtype::Get type) const noexcept {
    switch (type) {
        case enumtype::Get::Current:  return sorcery.cursed_energy;
        case enumtype::Get::Max:      return sorcery.max_cursed_energy;
        case enumtype::Get::Previous: return sorcery.previous_cursed_energy;
    }
    return -1.0;
}

void CurseUser::CursedEnergy(enumtype::Set type, double amount) {
    switch (type) {
        case enumtype::Set::Current:  
            sorcery.cursed_energy = amount;
            break;
        case enumtype::Set::Max:      
            sorcery.max_cursed_energy = amount;
            break;
        case enumtype::Set::Previous: 
            sorcery.previous_cursed_energy = amount;
            break;
    }
}

void CurseUser::Efficiency(CEfficiency type){
    ce_efficiency = type;
}