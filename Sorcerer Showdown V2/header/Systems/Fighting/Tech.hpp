#pragma once

struct TechAbility;
class Character;
class CurseUser;

struct Tech final {
    static bool ResolveOutput(TechAbility chosen_ct, CurseUser& user);
    static bool ResolveCursedEnergy(CurseUser& user, TechAbility chosen_ct, Character& target);
};