#pragma once
#include "../Structs.hpp"

#include <cstdint>

enum class HelpfulnessType : std::uint8_t {
    Offense,
    Boost,
    Protection,
    Regeneration
};

struct Shikigami final {
    EntityInfo id;
    double helpful_value{1.0};
    HelpfulnessType help_type{HelpfulnessType::Offense};
};