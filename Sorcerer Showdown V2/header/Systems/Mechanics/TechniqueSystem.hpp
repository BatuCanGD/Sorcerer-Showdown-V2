#pragma once
#include <utility>
#include <cstdint>
struct TechAbility;
struct Technique;
struct Character;


namespace TechniqueSystem  {
    enum class AbilityType : std::uint8_t { Strongest, LeastOutput, LeastCE, Random };
    std::pair<bool, double> ResolveOutput(TechAbility chosen_ct, Character& user);
    std::pair<bool, double> ResolveCursedEnergy(TechAbility chosen_ct, Character& user);
    double CalculateDamage(const TechAbility& tc, const Character& c);
    bool HasBarrier(const Technique& tech);
    TechAbility* GetAbility(Technique& tech, AbilityType type);
    TechAbility& ChooseAbility(Technique& tech);
    void ChantAbility(TechAbility& tech, const Character& cu);
};