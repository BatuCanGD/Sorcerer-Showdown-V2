#pragma once
#include <optional>
#include <memory>
#include <tuple>

class Character;
struct Domain;

namespace DomainHelper {
    std::tuple<bool, double> CalculateHit(const std::optional<Domain>& domain, const std::unique_ptr<Character>& c);
};