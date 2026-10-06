#include "../../header/CharacterType/CurseUser.hpp"

CurseUser::CurseUser() {};
CurseUser::~CurseUser() = default;

const JujutsuSystem& CurseUser::Jujutsu() const noexcept {
    return jujutsu;
}
JujutsuSystem& CurseUser::Jujutsu() noexcept {
    return jujutsu;
}
const CursedEnergySystem& CurseUser::CursedEnergy() const noexcept {
    return ce_system;
}
CursedEnergySystem& CurseUser::CursedEnergy() noexcept {
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

const CurseUser* CurseUser::CanUseSorcery() const noexcept {
    return this;
}

CurseUser* CurseUser::CanUseSorcery() noexcept {
    return this;
}

const ReverseCTSystem& CurseUser::RCT() const noexcept{
    return rct_system;
}

ReverseCTSystem& CurseUser::RCT() noexcept{
    return rct_system;
}

std::unique_ptr<Character> CurseUser::Clone() const {
    return std::make_unique<CurseUser>(*this);
}