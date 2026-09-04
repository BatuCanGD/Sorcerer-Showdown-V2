#pragma once

class Character;
struct AttackStruct;

namespace Log {
    void Attack(AttackStruct ats, Character& c1, Character& c2);
}