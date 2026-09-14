#pragma once

#include <optional>
#include <memory>
#include <utility>

class Character;
struct Domain;

enum class ClashWinner {
    None,
    First,
    Second,
    Both
};

enum class DomainWinCon {
    None,
    Overwhelmed,
    Refinement,
    Attrition
};

namespace DomainSystem {
    std::pair<bool, double> CalculateHit(const std::optional<Domain>& domain, const std::unique_ptr<Character>& c); 
    std::pair<ClashWinner, DomainWinCon> ClashDomains(std::optional<Domain>& first, std::optional<Domain>& second); // use case for already active domains
    
    void ResetDomain(std::optional<Domain>& domain);
    constexpr const char* DomainWinType(DomainWinCon win_condition);
    constexpr const char* DomainClashWinner(ClashWinner winner);
};