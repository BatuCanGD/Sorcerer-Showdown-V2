#include "../../header/CharacterType/CurseUser.hpp"


double CurseUser::CursedEnergy(type::Get type) const noexcept {
    switch (type) {
        case type::Get::Current:  return sorcery.cursed_energy;
        case type::Get::Max:      return sorcery.max_cursed_energy;
        case type::Get::Previous: return sorcery.previous_cursed_energy;
    }
    return -1.0;
}

void CurseUser::CursedEnergy(type::Set type, double amount) {
    switch (type) {
        case type::Set::Current:  
            sorcery.cursed_energy = amount;
            break;
        case type::Set::Max:      
            sorcery.max_cursed_energy = amount;
            break;
        case type::Set::Previous: 
            sorcery.previous_cursed_energy = amount;
            break;
    }
}

void CurseUser::Efficiency(CEfficiency type){
    ce_efficiency = type;
}