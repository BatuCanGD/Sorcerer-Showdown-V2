#pragma once
#include <utility>
#include <cstdint>

class Character;
class Technique;

struct CursedTool;
struct BindingVow;
struct Domain;
struct Shikigami;
struct Battlefield;

enum class TargetingType : std::uint8_t { // targeted character type
    LowestHP,
    HighestHP,
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

namespace AI {
    Character* GetTarget(const Character& user, const TargetingType tp, const Battlefield& bf);
    std::pair<bool, CursedTool> asd(); // forgot what this placeholder function was for
    void Fight(Character& user, Battlefield& bf, const FightingStyle fs, const ResourceUsage rs);
};