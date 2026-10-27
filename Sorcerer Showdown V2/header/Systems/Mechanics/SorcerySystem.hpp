#pragma once
#include "../../SorceryStruct.hpp"



namespace SorcerySystem {
    double ApplySpendingMultiplier(double amount, const Character& c);
    double OutputStatusMultiplier(Sorcery::OutputSystem::Status st) noexcept;
    double EfficiencyMultiplier(Sorcery::CursedEnergySystem::Efficiency type) noexcept;
    
    double ApplySixEyes(const double amount);
    double ApplyRCTCost(const double amount);
    double ApplyReinforcementCost(const double amount);

    bool CEMoreThanMax(Character& c, double p);
    void ApplyBurnOut(Character& c);
}