#pragma once
#include "../../CharacterType/CurseUser.hpp"

namespace SorcerySystem {
    double EfficiencyMultiplier(CursedEnergySystem::Efficiency type) noexcept;
    double ApplySixEyes(double amount);
}