#pragma once

#include <optional>
#include <memory>
#include <utility>
#include <cstdint>

class Character;
struct Domain;

enum class ClashWinner : std::uint8_t {
    None,
    First,
    Second,
    Both
};

enum class DomainWinCon : std::uint8_t {
    None,
    Overwhelmed,
    Refinement,
    Attrition
};

namespace DomainSystem {
    std::pair<bool, double> CalculateHit(const std::optional<Domain>& domain, const std::unique_ptr<Character>& c); 
    std::pair<ClashWinner, DomainWinCon> ClashDomains(std::optional<Domain>& first, std::optional<Domain>& second); // use case for already active domains
    void ResetDomain(std::optional<Domain>& domain);
};