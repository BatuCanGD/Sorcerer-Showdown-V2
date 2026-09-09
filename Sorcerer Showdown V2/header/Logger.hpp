#pragma once
#include "Systems/Fighting/CombatSystem.hpp"

class Character;
struct battlefield;
struct AttackStruct;
struct DamageStruct;

namespace Log {
    void Attack(AttackStruct ats, Character& c1, Character& c2);
    void Damage(DamageStruct dms, Character& attacked);
    void Death(battlefield& bf);
}