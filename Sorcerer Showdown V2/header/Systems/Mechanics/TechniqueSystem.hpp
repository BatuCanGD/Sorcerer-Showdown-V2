#pragma once
#include <utility>
#include <cstdint>

struct TechAbility;
struct Technique;
struct TechRequirement;
struct Character;

namespace Sorcery {
    struct CurseUserSystem;
}

enum class ReqFailReason : std::uint8_t {
    None,
    ChantLevelNotMet,
    StatNotMet
};

namespace TechniqueSystem  {
    enum class AbilityType : std::uint8_t { Strongest, LeastOutput, LeastCE, Random };
    std::pair<bool, double> ResolveOutput(const TechAbility& chosen_ct, const Character& user);
    std::pair<bool, double> ResolveCursedEnergy(const TechAbility& chosen_ct, const Character& user);
    std::pair<bool, ReqFailReason> MeetsRequirements(const TechAbility& ct, const Character& user);
    double CalculateDamage(const TechAbility& tc, const Character& c);
    double GetBarrierCost(const Technique& tech);
    bool HasBarrier(const Technique& tech);
    void SetBarrier(Sorcery::CurseUserSystem& tech, bool t);
    TechAbility* GetAbility(Technique& tech, AbilityType type);
    TechAbility& ChooseAbility(const Character& c, Technique& tech);
    void ChantAbility(TechAbility& tech, const Character& cu);
};