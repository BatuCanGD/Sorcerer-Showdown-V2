#pragma once
#include "../../CharacterType/CurseUser.hpp"

namespace SorcerySystem {
    double EfficiencyMultiplier(CursedEnergySystem::Efficiency type) noexcept;
    double ApplyRCTCost(const double amount);
    double ApplyReinforcementCost(const double amount);
    double ApplySixEyes(const double amount);
}