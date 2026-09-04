#pragma once

class Character;
struct battlefield;
struct AttackStruct;

namespace Log {
    void Attack(AttackStruct ats, Character& c1, Character& c2);
    void Death(battlefield& bf);
}