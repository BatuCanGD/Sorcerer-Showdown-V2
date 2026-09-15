#pragma once
#include "Systems/System/CombatSystem.hpp"

enum class ClashWinner : std::uint8_t;
enum class DomainWinCon : std::uint8_t;

class Character;
struct battlefield;
struct AttackStruct;
struct DamageStruct;

namespace Log {
    void Attack(const AttackStruct ats, const Character& c1, const Character& c2);
    void Damage(const DamageStruct dms, const Character& attacked);
    void Clash(const ClashWinner winner, const DomainWinCon win_con);
    void Death(const battlefield& bf);
}