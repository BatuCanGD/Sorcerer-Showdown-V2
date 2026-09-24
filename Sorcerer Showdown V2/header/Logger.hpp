#pragma once
#include "Systems/System/CombatSystem.hpp"

#include <vector>
#include <utility>

enum class ClashWinner : std::uint8_t;
enum class DomainWinCon : std::uint8_t;

class Character;
class CurseUser;
class Technique;

struct Battlefield;
struct StatusEffect;
struct AttackStruct;
struct DamageStruct;

namespace Log {
    enum class LogDetailType  : std::uint8_t { Basic, Detailed };
    enum class CTLogType      : std::uint8_t { Name, Abilities, Both };

    void CharacterInfo(const Character& c, const LogDetailType info_type = LogDetailType::Basic);
    void TechniqueInfo(const Technique& ct, const CTLogType log_type = CTLogType::Name, const LogDetailType info_type = LogDetailType::Basic);

    void Attack(const AttackStruct ats, const Character& c1, const Character& c2);
    void TechniqueAttack(const TechniqueStruct tc);
    void Damage(const DamageStruct dms, const Character& attacked);
    void Clash(const ClashWinner winner, const DomainWinCon win_con);
    void Effects(std::pair<const std::vector<StatusEffect>&, const Character&> p);
    void Death(const Battlefield& bf);
}