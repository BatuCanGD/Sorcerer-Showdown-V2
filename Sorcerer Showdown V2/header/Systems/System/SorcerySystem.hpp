#pragma once
#include "../../CharacterType/CurseUser.hpp"

namespace SorcerySystem {
    double ApplySpendingMultiplier(double amount, const CurseUser& c);
    double EfficiencyMultiplier(CursedEnergySystem::Efficiency type) noexcept;
    double ApplySixEyes(const double amount);

    double ApplyRCTCost(const double amount);
    double ApplyReinforcementCost(const double amount);
}