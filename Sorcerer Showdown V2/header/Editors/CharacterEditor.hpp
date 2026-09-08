#pragma once

#include "../CharacterType/Sorcerer.hpp"
#include "../CharacterType/CurseUser.hpp"

#include <memory>
#include <cstdint>

class Character;
class CurseUser;
class Sorcerer;

struct CharacterEditor final {
    // identity section
    static void SetIdentity(Character& c, CharIdentity cd);
    // base state section
    static void SetStats(Character& c, CharState cs);
    static void SetHealth(Character& c, double hp);
    static void SetInvulnerability(Character& c, bool t);
    static void SetDurability(Character& c, double dr);
    static void SetStrength(Character& c, double str);
    // inventory
    enum class ItemPlacement : std::uint8_t { OnHand, Offhand, Inventory };
    static void GiveCharacterTool(Character& c, std::unique_ptr<int> tool, ItemPlacement place);
    static void SetInventoryAccess(Character& c, bool t);
    /*                        Character End                       */
    // inside curse user
    static void SetCursedEnergyEfficiency(CurseUser& c, CurseUserSystem::Efficiency efficiency);
    // curse user system
    static void SetCurseUserSystem(CurseUser& c, CurseUserSystem cus);
    static void SetCursedEnergy(CurseUser& c, double ce);
    static void SetBlackFlashChance(CurseUser& c, int ch);
    static void AddBindingVow(CurseUser& c, std::unique_ptr<int> vow);
    static void AddShikigami(CurseUser& c, std::unique_ptr<int> shk);
    static void SetTechnique(CurseUser& c, std::unique_ptr<Technique> tech);
    static void SetDomain(CurseUser& c, std::unique_ptr<int> domain);
    static void SetDomainNullifier(CurseUser& c, std::unique_ptr<int> dnull);
    // traits
    static void SetTraitSixEyes(CurseUser& c, bool t);
    static void SetTraitPassiveHealing(CurseUser& c, bool t);
    /*                        CurseUser End                       */
    // sorcerer system
    static void SetReverseCursedTechnique(Sorcerer& c, bool can_use);
    static void SetReverseCursedTechniqueLevel(Sorcerer& c, SorcererSystem::RCTLevel lvl);
    /*                        Sorcerer End                        */
};