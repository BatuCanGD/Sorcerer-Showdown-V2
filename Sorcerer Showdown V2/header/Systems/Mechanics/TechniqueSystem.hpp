#pragma once
#include <utility>
#include <cstdint>
struct TechAbility;
class Technique;
class Character;
class CurseUser;

namespace TechniqueSystem  {
    enum class AbilityType : std::uint8_t { Strongest, LeastOutput, LeastCE, Random };
    std::pair<bool, double> ResolveOutput(TechAbility chosen_ct, CurseUser& user);
    std::pair<bool, double> ResolveCursedEnergy(CurseUser& user, TechAbility chosen_ct, Character& target);
    bool HasBarrier(const Technique& tech);
    TechAbility* GetAbility(Technique& tech, AbilityType type);
    TechAbility& ChooseAbility(Technique& tech);
    void ChantAbility(TechAbility& tech);
};