#pragma once
#include <cstdint>

class Character;
struct Battlefield;

enum class TargetingType : std::uint8_t {
    LowestHP,
    HighestHP,
    Mixed
};

enum class FightingStyle : std::uint8_t {
    Aggressive,
    Defensive,
    Conservative,
    Mixed
};

namespace CharacterAI {
    Character* GetTarget(const TargetingType& tp, Battlefield&);
    void Fight(Character&, const FightingStyle& fs);
};