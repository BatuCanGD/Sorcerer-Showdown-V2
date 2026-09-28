#pragma once

#include "../CharacterType/CurseUser.hpp"

#include <optional>
#include <cstdint>

class Character;
class CurseUser;

struct CursedTool;
struct Shikigami;
struct BindingVow;
struct EntityId;

namespace CharacterEditor {
    // identity section
    void SetIdentity(Character& c, EntityInfo cd);
    void SetStyle(Character& c, BattleIQ iq);
    // base state section
    void SetStats(Character& c, CharState cs);
    void SetHealth(Character& c, double hp);
    void SetInvulnerability(Character& c, bool t);
    void SetDurability(Character& c, double dr);
    void SetStrength(Character& c, double str);
    // inventory
    enum class Placement : std::uint8_t { OnHand, Offhand, Inventory };
    void GiveCharacterTool(Character& c, std::optional<CursedTool> tool, Placement place);
    void SetInventoryAccess(Character& c, bool t);
    /*                        Character End                       */
    // inside curse user
    void SetCursedEnergyEfficiency(CurseUser& c, CursedEnergySystem::Efficiency efficiency);
    // curse user system
    void SetCurseUserSystem(CurseUser& c, CursedEnergySystem cus);
    void SetCursedEnergy(CurseUser& c, double ce);
    void SetBlackFlashChance(CurseUser& c, int ch);
    void AddBindingVow(CurseUser& c, BindingVow vow);
    void AddShikigami(CurseUser& c, Shikigami shk);
    void SetTechnique(CurseUser& c, Technique tech);
    void SetDomain(CurseUser& c, Domain domain);
    void SetDomainNullifier(CurseUser& c, Neutralizer neutralizer);
    // traits
    void SetTraitSixEyes(CurseUser& c, bool t);
    void SetTraitPassiveHealing(CurseUser& c, bool t);
    void SetReverseCursedTechnique(CurseUser& c, bool can_use);
    void SetReverseCursedTechniqueLevel(CurseUser& c, ReverseCTSystem::RCTLevel lvl);
};