#pragma once
#include "../CharacterType/CurseUser.hpp"

namespace Sorcery {
    double EfficiencyMultiplier(CursedEnergySystem::Efficiency type) noexcept;
    double ApplySixEyes(double amount);
}