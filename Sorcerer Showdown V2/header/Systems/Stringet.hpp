#pragma once
#include "../CharacterType/CurseUser.hpp"

#include <string>


namespace Stringet {
    const std::string HealthStr(const double hp);
    const std::string DurabilityStr(const double dr);
    const std::string StrengthStr(const double str);
    const std::string CursedEnergyStr(const double str);
    const std::string EfficiencyStr(const CursedEnergySystem::Efficiency& efficiency);

    const std::string OutputStr(const double max);                      // "X"
    const std::string OutputCmpStr(const double cur, const double max); // "X/Y"
}