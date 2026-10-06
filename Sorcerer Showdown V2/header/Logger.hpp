#pragma once
#include "Systems/Mechanics/CombatSystem.hpp"
#include "Systems/Mechanics/DomainSystem.hpp"

#include <vector>

enum class ClashWinner : std::uint8_t;
enum class DomainWinCon : std::uint8_t;

class Character;
class CurseUser;
class Technique;

struct Battlefield;
struct SurehitStruct;
struct StatusEffect;
struct AttackStruct;
struct DamageStruct;

namespace Log {
    enum class LogDetailType  : std::uint8_t { Basic, Detailed };
    enum class CTLogType      : std::uint8_t { Name, Abilities, Both };
    enum class SurehitHit     : std::uint8_t { Person, Neutralizer };

    void CharacterInfo(const Character& c);
    void TechniqueInfo(const Technique& ct, const CTLogType log_type = CTLogType::Name, const LogDetailType info_type = LogDetailType::Basic);

    void DomainSurehit(const SurehitStruct st, const SurehitHit ht = SurehitHit::Person);
    void Attack(const AttackStruct ats, const Character& c1, const Character& c2);
    void TechniqueAttack(const TechniqueStruct tc);
    void Damage(const DamageStruct dms, const Character& attacked);
    void Clash(const DomainClashStruct ds);
    void Effects(const std::vector<StatusEffect>&, const Character& p);
    void Death(const Battlefield& bf);
}