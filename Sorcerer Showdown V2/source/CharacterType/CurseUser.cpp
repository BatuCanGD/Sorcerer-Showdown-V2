#include "../../header/CharacterType/CurseUser.hpp"


double CurseUser::CursedEnergy(type::Get type) const noexcept {
    switch (type) {
        case type::Get::Current:  return sorcery.cursed_energy;
        case type::Get::Max:      return sorcery.max_cursed_energy;
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
    }
}

void CurseUser::CursedEnergy(type::Expend type, double amount) {
    switch (type) {
        case type::Expend::Current:  
            sorcery.cursed_energy -= amount;
            break;
        case type::Expend::Max:      
            sorcery.max_cursed_energy -= amount;
            break;
    }
}
void CurseUser::CursedEnergy(type::Add type, double amount) {
    switch (type) {
        case type::Add::Current:  
            sorcery.cursed_energy += amount;
            break;
        case type::Add::Max:      
            sorcery.max_cursed_energy += amount;
            break;
    }
}


double CurseUser::Output(type::Get t) const noexcept {
    switch(t){
        case type::Get::Current:    return sorcery.current_output;
        case type::Get::Max:        return sorcery.max_output_potential;
        default:                    return sorcery.current_output;
    }
}
double CurseUser::Output(type::Type t, double amount){
    switch(t){
        case type::Type::Add: 
            sorcery.current_output += amount;
            break;
        case type::Type::Expend: 
            sorcery.current_output -= amount;
            break;
        case type::Type::Get: 
            return sorcery.current_output;
        case type::Type::Set: 
            sorcery.current_output = amount;
            break;
    }
    return 0.0;
}
void CurseUser::Output(type::Set t, double amount){
    switch(t){
        case type::Set::Current: 
            sorcery.current_output = amount;
            break;
        case type::Set::Max: 
            sorcery.max_output_potential = amount;
            break;
    }
}

Technique* CurseUser::Technique() const {
    return jujutsu.technique.get();
}

void CurseUser::Efficiency(CEfficiency type){
    ce_efficiency = type;
}
CurseUser::CEfficiency CurseUser::Efficiency() const noexcept {
    return ce_efficiency;
}

bool CurseUser::CanUseSorcery() const {
    return true;
}
bool CurseUser::HasSixEyes() const noexcept {
    return traits.six_eyes;
}