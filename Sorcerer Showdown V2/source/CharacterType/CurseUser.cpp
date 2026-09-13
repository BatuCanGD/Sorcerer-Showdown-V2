#include "../../header/CharacterType/CurseUser.hpp"

#include <stdexcept>

const JujutsuSystem& CurseUser::Jujutsu() const noexcept {
    return jujutsu;
}
JujutsuSystem& CurseUser::Jujutsu() noexcept {
    return jujutsu;
}
const CursedEnergySystem& CurseUser::CursedEnergySys() const noexcept {
    return ce_system;
}
CursedEnergySystem& CurseUser::CursedEnergySys() noexcept {
    return ce_system;
}
const CurseUserAmplification& CurseUser::Amplification() const noexcept {
    return amplification;
}
CurseUserAmplification& CurseUser::Amplification() noexcept {
    return amplification;
}
const CurseUserOutput& CurseUser::Output() const noexcept {
    return output;
}
CurseUserOutput& CurseUser::Output() noexcept {
    return output;
}
const SorceryTrait& CurseUser::Traits() const noexcept {
    return traits;
}
SorceryTrait& CurseUser::Traits() noexcept {
    return traits;
}


double CurseUser::CursedEnergy(ValType type) const noexcept {
    switch (type) {
        case ValType::Current:  return ce_system.cursed_energy;
        case ValType::Maximum:  return ce_system.max_cursed_energy;
    }
    return -1.0;
}

void CurseUser::CursedEnergy(OpType type, double amount) {
    if (amount <= 0.0){
        throw std::range_error("Cannot use 0 or negative amounts");
    }
    switch (type) {
        case OpType::Add:  
            ce_system.cursed_energy += amount;
            break;
        case OpType::Expend:
            ce_system.cursed_energy -= amount;
            break;
        case OpType::Set:
            ce_system.cursed_energy = amount;
            break;
        case OpType::Get:
            throw std::invalid_argument("This overload function is not a getter");
    }
    if (ce_system.cursed_energy > ce_system.max_cursed_energy){
        ce_system.cursed_energy = ce_system.max_cursed_energy;
    }
}

const CurseUser* CurseUser::CanUseSorcery() const noexcept {
    return this;
}

bool CurseUser::HasSixEyes() const noexcept {
    return traits.six_eyes;
}