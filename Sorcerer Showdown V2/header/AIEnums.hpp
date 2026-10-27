#pragma once
#include <cstdint>

enum class TargetingType : std::uint8_t { // targeted character type
    LowestHP,
    HighestHP,
    Random
};

enum class TargetedTypes : std::uint8_t { // what type of target the character chooses
    Character,
    Mixed
};

enum class FightingStyle : std::uint8_t { // how the character approaches a conclusion
    Aggressive,
    Defensive,
    Mixed
};

enum class ResourceUsage : std::uint8_t { // techniques, domains, shikigami, cursed tools
    AllOut,
    Mixed,
    Conservative
};