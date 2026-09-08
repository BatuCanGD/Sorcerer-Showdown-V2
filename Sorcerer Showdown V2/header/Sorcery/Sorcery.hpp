#pragma once
#include "../CharacterType/CurseUser.hpp"

namespace Sorcery {
    double EfficiencyMultiplier(CurseUserSystem::Efficiency type) noexcept;
    double ApplySixEyes(double amount);
}