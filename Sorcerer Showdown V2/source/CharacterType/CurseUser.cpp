#include "../../header/CharacterType/CurseUser.hpp"

#include <stdexcept>

const JujutsuSystem& CurseUser::Jujutsu() const noexcept {
    return jujutsu;
}
const CursedEnergySystem& CurseUser::CursedEnergySys() const noexcept {
    return ce_system;
}
const SorceryTrait& CurseUser::Traits() const noexcept {
    return traits;
}

double CurseUser::CursedEnergy(type::Get type) const noexcept {
    switch (type) {
        case type::Get::Current:  return ce_system.cursed_energy;
        case type::Get::Max:      return ce_system.max_cursed_energy;
    }
    return -1.0;
}

void CurseUser::CursedEnergy(type::Type type, double amount) {
    if (amount <= 0.0){
        throw std::range_error("Cannot use 0 or negative amounts");
    }
    switch (type) {
        case type::Type::Add:  
            state.health += amount;
            break;
        case type::Type::Expend:
            state.health -= amount;
            break;
        case type::Type::Set:
            state.health = amount;
            break;
        case type::Type::Get:
            throw std::invalid_argument("This overload function is not a getter");
    }
    if (ce_system.cursed_energy > ce_system.max_cursed_energy){
        ce_system.cursed_energy = ce_system.max_cursed_energy;
    }
}

void Character::Health(type::Type type, double amount) {
    if (amount <= 0.0){
        throw std::range_error("Cannot use 0 or negative amounts");
    }
    switch (type) {
        case type::Type::Add:  
            state.health += amount;
            break;
        case type::Type::Expend:
            state.health -= amount;
            break;
        case type::Type::Set:
            state.health = amount;
            break;
        case type::Type::Get:
            throw std::invalid_argument("This overload function is not a getter");
    }
    if (state.health > state.max_health){
        state.health = this->Health(type::Get::Max);
    }
}

const CurseUser* CurseUser::CanUseSorcery() const noexcept {
    return this;
}

bool CurseUser::HasSixEyes() const noexcept {
    return traits.six_eyes;
}