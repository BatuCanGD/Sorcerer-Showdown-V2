#pragma once
#include <cstdint>

class Character;
class CurseUser;

struct Technique;
struct CursedTool;
struct CharInv;
struct BattleIQ;
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
    Character* GetTarget(const Character& user, const TargetingType& tp, const Battlefield& bf);

    void DF_UseDomain(CurseUser& c, const Battlefield& bf, const FightingStyle& fs);
    void DF_UseTechnique(CurseUser& c, Character& target, const BattleIQ& bq);

    void DRM_UseShikigami(CurseUser& c, const ResourceUsage& rs);
    void DRM_UseBindingVow(CurseUser& c, const Character& tr, const Battlefield& bf, const ResourceUsage& iq);
    void DRM_UseReinforcement(CurseUser& c, const ResourceUsage& rs);
    void DRM_UseRCT(CurseUser& c, const ResourceUsage& rs);
    void DRM_UseTools(Character& c, const Character& cc);

    void DoFighting(Character& user, Character* target, const Battlefield& bf, const BattleIQ& iq);
    void DoResourceManagement(Character& user, const Character& tr ,const Battlefield& bf, const ResourceUsage& ru);

    void Fight(Character& user, Battlefield& bf);
};