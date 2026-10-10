#pragma once
#include <cstdint>

class Character;
class CurseUser;
class Technique;

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
    
    enum class WeaponPlacement : std::uint8_t { Hand, Offhand, Inventory };
    enum class WeaponChoice : std::uint8_t { None, HighestDamage, EffectInducing, TechniqueBypassing };
    CursedTool GetFromInv(CharInv& inv, WeaponChoice wc);
    CursedTool GetWeapon(CharInv& inv, WeaponPlacement which, WeaponChoice which_type);
    void MoveWeapon(CharInv& inv, CursedTool wp, WeaponPlacement where);
    void SwitchWeapons(CharInv& inv, WeaponPlacement which, WeaponPlacement where, WeaponChoice wc);
    bool HasWeapon(CharInv& c,WeaponChoice wc);

    void RM_UseShikigami(CurseUser& c, const ResourceUsage& rs);
    void RM_UseReinforcement(CurseUser& c, const ResourceUsage& rs);
    void RM_UseRCT(CurseUser& c, const ResourceUsage& rs);
    void RM_UseTools(Character& c, const Character& cc);

    void DoFighting(Character& user, Character* target, const BattleIQ& iq);
    void DoResourceManagement(Character& user, const Character& tr ,const Battlefield& bf, const ResourceUsage& ru);

    void Fight(Character& user, Battlefield& bf);
};