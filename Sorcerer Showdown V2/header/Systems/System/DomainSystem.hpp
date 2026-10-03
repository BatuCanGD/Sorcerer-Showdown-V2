#pragma once
#include "../../Enums.hpp"
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

namespace DomainSystem {
    DomainHitStruct CalculateHit(const std::optional<Domain>& domain, const std::unique_ptr<Character>& c); 
    DomainClashStruct ClashDomains(std::optional<Domain>& first, std::optional<Domain>& second); // use case for already active domains
    void HandleSureHit(Character& c, const double damage, const bool does_paralyze, const globalums::DamageType dt); 
    void ResetDomain(std::optional<Domain>& domain);
};