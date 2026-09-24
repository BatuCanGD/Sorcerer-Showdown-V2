#pragma once
#include <utility>
#include <vector>
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

struct AIConclusion final {
    std::vector<Shikigami*> used_shikigami;
    Character* attacker;
    Character* attacked;
    BindingVow* used_vow;
    Technique* used_technique;
    CursedTool* used_tool;
    Domain* used_domain;
    bool attack_did_hit;
};

namespace AI {
    Character* GetTarget(const Character& user, const TargetingType tp, const Battlefield& bf);
    std::pair<bool, CursedTool> asd(); // forgot what this placeholder function was for
    AIConclusion Fight(Character& user, Battlefield& bf, const FightingStyle fs, const ResourceUsage rs);
};