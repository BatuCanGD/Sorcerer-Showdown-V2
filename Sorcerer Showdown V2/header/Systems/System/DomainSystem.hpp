#pragma once
#include <optional>
#include <memory>
#include <cstdint>

class Character;
class CurseUser;
struct Domain;

enum class ClashWinner : std::uint8_t {
    None,
    First,
    Second,
    Both
};

enum class DomainWinCon : std::uint8_t {
    None,
    Refinement,
    Attrition
};

struct DomainHitStruct final {
    double damage;
    bool does_hit;
};
struct DomainClashStruct final {
    ClashWinner winner;
    DomainWinCon win_condition;
};
struct SurehitStruct final {
    double damage;
    const Character* c;
    const Domain* dm;
};

namespace DomainSystem {
    double CalculateHitDamage(const std::optional<Domain>& domain, const std::unique_ptr<Character>& c);
    bool CalculateActualHit(const std::optional<Domain>& domain, const std::unique_ptr<Character>& c); 
    DomainClashStruct ClashDomains(std::optional<Domain>& first, std::optional<Domain>& second); // use case for already active domains
    SurehitStruct HandleSureHit(Character& c, const std::optional<Domain>& dm); 
    void ResetDomain(std::optional<Domain>& domain);
};