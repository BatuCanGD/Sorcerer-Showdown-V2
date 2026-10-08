#pragma once
#include <cstdint>

class Character;
class Technique;

struct CursedTool;
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
    
    enum class WeaponPlacement : std::uint8_t { Hand, Offhand, Inventory };
    enum class WeaponChoice : std::uint8_t { None, HighestDamage, EffectInducing, TechniqueBypassing };
    void SwitchWeapons(Character& c, WeaponPlacement which, WeaponPlacement where, WeaponChoice wc);

    void DoFighting(Character& user, Character* target, const BattleIQ& iq);
    void DoResourceManagement(Character& user, const Battlefield& bf, const ResourceUsage& ru);

    void Fight(Character& user, Battlefield& bf);
};