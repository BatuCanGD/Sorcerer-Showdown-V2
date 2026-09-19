#include "../../header/Editors/CharacterEditor.hpp"
#include "../../header/CharacterType/Character.hpp"
#include "../../header/CharacterType/CurseUser.hpp"
#include "../../header/Stuff/CursedTool.hpp"
#include "../../header/Sorcery/Shikigami.hpp"

#include <print>
#include <utility>

// identity
void CharacterEditor::SetIdentity(Character &c, EntityInfo cd){
    c.Identity() = cd;
}
// base state
void CharacterEditor::SetStats(Character& c, CharState cs) {
    c.State() = cs;
}
void CharacterEditor::SetHealth(Character& c, double hp){
    c.State().max_health = hp;
    c.State().health = hp;
}
void CharacterEditor::SetInvulnerability(Character& c, bool t){
    c.State().is_invulnerable = t;
}
void CharacterEditor::SetDurability(Character &c, double dr){
    c.State().durability = dr;
}
void CharacterEditor::SetStrength(Character &c, double str){
    c.State().strength = str;
}
// inventory
void CharacterEditor::GiveCharacterTool(Character& c, std::optional<WeaponType> tool, Placement place){
    switch(place){
        case Placement::OnHand:
            if (c.Equipment().current_tool){
                std::println(stderr, "Character already has a current item, item will be overwritten");
            }
            c.Equipment().current_tool = std::move(tool);
            break;
        case Placement::Offhand:
            if (c.Equipment().stored_tool){
                std::println(stderr, "Character already has a stored item, item will be overwritten");
            }
            c.Equipment().stored_tool = std::move(tool);
            break;
        case Placement::Inventory:
            if (tool){
                c.Equipment().inventory.push_back(std::move(*tool));
            }
            break;
    }
}
void CharacterEditor::SetInventoryAccess(Character& c, bool t){
    c.Equipment().has_access_to_inventory = t;
}
// curse user system
void CharacterEditor::SetCursedEnergyEfficiency(CurseUser& c, CursedEnergySystem::Efficiency efficiency){
    c.Sorcery().efficiency = efficiency;
}
void CharacterEditor::SetCurseUserSystem(CurseUser &c, CursedEnergySystem cus){
    c.Sorcery() = cus;
}
void CharacterEditor::SetCursedEnergy(CurseUser &c, double ce){
    c.Sorcery().max_cursed_energy = ce;
    c.Sorcery().cursed_energy = ce;
}
void CharacterEditor::SetBlackFlashChance(CurseUser& c, int ch){
    c.Sorcery().bf_chance = ch;
}
void CharacterEditor::AddBindingVow(CurseUser& c, BindingVow vow){
    c.Jujutsu().binding_vows.push_back(std::move(vow));
}
void CharacterEditor::AddShikigami(CurseUser& c, Shikigami shk){
    c.Jujutsu().shikigami.push_back(std::move(shk));
}
void CharacterEditor::SetTechnique(CurseUser& c, Technique tech){
    c.Jujutsu().technique = std::move(tech);
}
void CharacterEditor::SetDomain(CurseUser& c, Domain domain){
    c.Jujutsu().domain = std::move(domain);
}
void CharacterEditor::SetDomainNullifier(CurseUser& c, Neutralizer dnf){
    c.Jujutsu().domain_neutralizer = std::move(dnf);
}
// traits
void CharacterEditor::SetTraitSixEyes(CurseUser& c, bool t){
    c.Traits().six_eyes = t;
}
void CharacterEditor::SetTraitPassiveHealing(CurseUser& c, bool t){
    c.Traits().passive_healing = t;
}
// Reverse Cursed Technique
void CharacterEditor::SetReverseCursedTechnique(CurseUser& c, bool t){
    c.RCTSystem().can_use_rct = t;
}
void CharacterEditor::SetReverseCursedTechniqueLevel(CurseUser& c, ReverseCTSystem::RCTLevel lvl){
    c.RCTSystem().rct_level = lvl;
}