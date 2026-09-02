#pragma once

#include "CharacterType/Sorcerer.hpp"
#include "CharacterType/CurseUser.hpp"

#include <string_view>
#include <memory>

class Character;
class CurseUser;
class Sorcerer;

enum class ItemPlacement {
    OnHand,
    Offhand,
    Inventory
};

struct CharacterEditor {
    // identity section
    static void SetName(Character& c, std::string_view name);
    static void SetColor(Character& c, std::string_view color);
    // base state section
    static void SetMaxHealth(Character& c, double hp);
    static void SetInvulnerability(Character& c, bool t);
    // inventory
    static void GiveCharacterTool(Character& c, std::unique_ptr<int> tool, ItemPlacement place);
    static void SetInventoryAccess(Character& c, bool t);
    /*                        Character End                       */
    // inside curse user
    static void SetCursedEnergyEfficiency(CurseUser& c, CurseUser::CEfficiency efficiency);
    // curse user system
    static void AddBindingVow(CurseUser& c, std::unique_ptr<int> vow);
    static void AddShikigami(CurseUser& c, std::unique_ptr<int> shk);
    static void SetTechnique(CurseUser& c, std::unique_ptr<int> tech);
    static void SetDomain(CurseUser& c, std::unique_ptr<int> domain);
    static void SetDomainNullifier(CurseUser& c, std::unique_ptr<int> dnull);
    // traits
    static void SetTraitSixEyes(CurseUser& c, bool t);
    static void SetTraitPassiveHealing(CurseUser& c, bool t);
    /*                        CurseUser End                       */
    // sorcerer system
    static void SetReverseCursedTechnique(Sorcerer& c, bool can_use);
    static void SetReverseCursedTechniqueLevel(Sorcerer& c, Sorcerer::RCTLevel lvl);
};