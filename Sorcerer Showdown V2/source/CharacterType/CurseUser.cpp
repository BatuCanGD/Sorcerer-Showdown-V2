#include "../../header/CharacterType/CurseUser.hpp"


double CurseUser::CursedEnergy(Get type) const noexcept {
    switch (type) {
        case Get::Current:  return sorcery.cursed_energy;
        case Get::Max:      return sorcery.max_cursed_energy;
        case Get::Previous: return sorcery.previous_cursed_energy;
    }
    return -1.0;
}

void CurseUser::CursedEnergy(Set type, double amount) {
    switch (type) {
        case Set::Current:  sorcery.cursed_energy = amount;
        case Set::Max:      sorcery.max_cursed_energy = amount;
        case Set::Previous: sorcery.previous_cursed_energy = amount;
    }
}

void CurseUser::Efficiency(CEfficiency type){
    ce_efficiency = type;
}