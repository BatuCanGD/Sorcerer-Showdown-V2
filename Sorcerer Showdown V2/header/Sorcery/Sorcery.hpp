#pragma once
#include "../CharacterType/CurseUser.hpp"

namespace Sorcery {
    double EfficiencyMultiplier(CurseUser::CEfficiency type) noexcept;
    double ApplySixEyes(double amount);
}