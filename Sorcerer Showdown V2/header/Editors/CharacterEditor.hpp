#pragma once

#include "../CharacterType/Sorcerer.hpp"
#include "../CharacterType/CurseUser.hpp"

#include <optional>
#include <cstdint>

class Character;
class CurseUser;
class Sorcerer;

struct WeaponType;
struct Shikigami;
struct BindingVow;
struct EntityId;

struct CharacterEditor final {
    // identity section
    static void SetIdentity(Character& c, EntityId cd);
    // base state section
    static void SetStats(Character& c, CharState cs);
    static void SetHealth(Character& c, double hp);
    static void SetInvulnerability(Character& c, bool t);
    static void SetDurability(Character& c, double dr);
    static void SetStrength(Character& c, double str);
    // inventory
    enum class Placement : std::uint8_t { OnHand, Offhand, Inventory };
    static void GiveCharacterTool(Character& c, std::optional<WeaponType> tool, Placement place);
    static void SetInventoryAccess(Character& c, bool t);
    /*                        Character End                       */
    // inside curse user
    static void SetCursedEnergyEfficiency(CurseUser& c, CursedEnergySystem::Efficiency efficiency);
    // curse user system
    static void SetCurseUserSystem(CurseUser& c, CursedEnergySystem cus);
    static void SetCursedEnergy(CurseUser& c, double ce);
    static void SetBlackFlashChance(CurseUser& c, int ch);
    static void AddBindingVow(CurseUser& c, BindingVow vow);
    static void AddShikigami(CurseUser& c, Shikigami shk);
    static void SetTechnique(CurseUser& c, std::optional<Technique> tech);
    static void SetDomain(CurseUser& c, std::optional<int> domain);
    static void SetDomainNullifier(CurseUser& c, std::optional<int> dnull);
    // traits
    static void SetTraitSixEyes(CurseUser& c, bool t);
    static void SetTraitPassiveHealing(CurseUser& c, bool t);
    /*                        CurseUser End                       */
    // sorcerer system
    static void SetReverseCursedTechnique(Sorcerer& c, bool can_use);
    static void SetReverseCursedTechniqueLevel(Sorcerer& c, ReverseCTSystem::RCTLevel lvl);
    /*                        Sorcerer End                        */
};