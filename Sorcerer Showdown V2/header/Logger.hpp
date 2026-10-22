#pragma once
#include "Systems/Mechanics/CombatSystem.hpp"
#include "Systems/Mechanics/DomainSystem.hpp"

#include <format>
#include <vector>
#include <variant>
#include <string_view>

class Character;
class CurseUser;

struct Shikigami;
struct Technique;
struct Battlefield;
struct SurehitStruct;
struct StatusEffect;
struct ToolStruct;
struct AttackStruct;
struct DamageStruct;

namespace Log {
    enum class LogDetailType  : std::uint8_t { Basic, Detailed };
    enum class CTLogType      : std::uint8_t { Name, Abilities, Both };
    enum class SurehitHit     : std::uint8_t { Person, Neutralizer };

    void WriteMessage(bool newline, std::string_view sv, std::format_args ags);
    void WriteError(std::string_view sv, std::format_args ags);

    template <typename... Args>
    inline void Message(std::format_string<Args...> f, Args&&... a){
        WriteMessage(false, f.get(), std::make_format_args(a...));
    }
    template <typename... Args>
    inline void Line(std::format_string<Args...> f = "", Args&&... a){
        WriteMessage(true, f.get(), std::make_format_args(a...));
    }
    template <typename... Args>
    inline void Error(std::format_string<Args...> f, Args&&... a) {
        WriteError(f.get(), std::make_format_args(a...));
    }

    void CharacterPresentation(const Character& c);
    void TechniqueInfo(const Technique& ct, const CTLogType log_type = CTLogType::Name, const LogDetailType info_type = LogDetailType::Basic);
    void DomainSurehit(const SurehitStruct st, const SurehitHit ht = SurehitHit::Person);
    void Attack(const std::variant<AttackStruct, ToolStruct> ats, const Character& c1, const std::variant<Character*, Shikigami*> c2);
    void TechniqueAttack(const TechniqueStruct tc);
    void TechniqueChant(const CurseUser& cu, const TechAbility& tc);
    void Damage(const DamageStruct dms, const Character& attacked);
    void Clash(const DomainClashStruct ds);
    void Effects(const std::vector<StatusEffect>&, const Character& p);
    void Death(const Battlefield& bf);
    void Clear();
}