#pragma once

struct TechAbility;
class Character;
class CurseUser;

struct Tech final
{
    static bool ResolveAbilityUsage(TechAbility tc, CurseUser& user, Character& target);
};