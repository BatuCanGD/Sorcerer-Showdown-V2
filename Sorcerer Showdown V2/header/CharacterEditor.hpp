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
    void SetName(Character& c, std::string_view name);
    void SetColor(Character& c, std::string_view color);
    // base state section
    void SetMaxHealth(Character& c, double hp);
    void SetInvulnerability(Character& c, bool t);
    // inventory
    void GiveCharacterTool(Character& c, std::unique_ptr<int> tool, ItemPlacement place);
    void SetInventoryAccess(Character& c, bool t);
    /*                        Character End                       */
    // inside curse user
    void SetCursedEnergyEfficiency(CurseUser& c, CurseUser::CEfficiency efficiency);
    // curse user system
    void AddBindingVow(CurseUser& c, std::unique_ptr<int> vow);
    void AddShikigami(CurseUser& c, std::unique_ptr<int> shk);
    void SetTechnique(CurseUser& c, std::unique_ptr<int> tech);
    void SetDomain(CurseUser& c, std::unique_ptr<int> domain);
    void SetDomainNullifier(CurseUser& c, std::unique_ptr<int> dnull);
    // traits
    void SetTraitSixEyes(CurseUser& c, bool t);
    void SetTraitPassiveHealing(CurseUser& c, bool t);
    /*                        CurseUser End                       */
    // sorcerer system
    void SetReverseCursedTechnique(Sorcerer& c, bool can_use);
    void SetReverseCursedTechniqueLevel(Sorcerer& c, Sorcerer::RCTLevel lvl);
};