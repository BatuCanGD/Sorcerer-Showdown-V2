#pragma once
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
    const Domain* first;
    const Domain* second;
    double f_damage;
    double s_damage;
    ClashWinner winner;
    DomainWinCon win_condition;
};
struct SurehitStruct final {
    double damage;
    const Character* c;
    const Domain* dm;
};

namespace DomainSystem {
    double CalculateHitDamage(const Domain& domain, const Character& c);
    bool CalculateActualHit(const Domain& domain, const Character& c); 
    DomainClashStruct ClashDomains(CurseUser& first, CurseUser& second); // use case for already active domains
    SurehitStruct HandleSureHit(Character& c, const Domain& dm); 
    bool HandleDamage(Domain& dm, const double d);
    void ResetDomain(Domain& domain, CurseUser* crs = nullptr);
};