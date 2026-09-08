#pragma once

#include <utility>

struct TechAbility;
class Technique;
class Character;
class CurseUser;

struct TechniqueSystem final {
    static std::pair<bool, double> ResolveOutput(TechAbility chosen_ct, CurseUser& user);
    static std::pair<bool, double> ResolveCursedEnergy(CurseUser& user, TechAbility chosen_ct, Character& target);
    static TechAbility ChooseAbility(Technique& tech);
};