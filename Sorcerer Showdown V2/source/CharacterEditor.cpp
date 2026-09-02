#include "../header/CharacterEditor.hpp"
#include "../header/CharacterType/Character.hpp"
#include "../header/CharacterType/CurseUser.hpp"
#include "../header/CharacterType/Sorcerer.hpp"

#include <print>
#include <utility>
#include <string_view>

// identity
void CharacterEditor::SetName(Character& c, std::string_view name){
    c.identity.name = name;
}
void CharacterEditor::SetColor(Character& c, std::string_view color){
    c.identity.color = color;
}
// base state
void CharacterEditor::SetMaxHealth(Character& c, double hp){
    c.state.max_health = hp;
    if (c.state.health > c.state.max_health){
        c.state.health = c.state.max_health;
    }
}
void CharacterEditor::SetInvulnerability(Character& c, bool t){
    c.state.is_invulnerable = t;
}
// inventory
void CharacterEditor::GiveCharacterTool(Character& c, std::unique_ptr<int> tool, ItemPlacement place){
    switch(place){
        case ItemPlacement::OnHand:
            if (c.equipment.current_tool != nullptr){
                std::println(stderr, "Character already has a current item, item will be overwritten");
            }
            c.equipment.current_tool = std::move(tool);
            break;
        case ItemPlacement::Offhand:
            if (c.equipment.stored_tool != nullptr){
            std::println(stderr, "Character already has a stored item, item will be overwritten");
            }
            c.equipment.stored_tool = std::move(tool);
            break;
        case ItemPlacement::Inventory:
            c.equipment.inventory.push_back(std::move(tool));
            break;
    }
}
void CharacterEditor::SetInventoryAccess(Character& c, bool t){
    c.equipment.has_access_to_inventory = t;
}
// inside curseuser
void CharacterEditor::SetCursedEnergyEfficiency(CurseUser& c, CurseUser::CEfficiency efficiency){
    c.ce_efficiency = efficiency;
}
// curse user system
void CharacterEditor::AddBindingVow(CurseUser& c, std::unique_ptr<int> vow){
    c.jujutsu.binding_vows.push_back(std::move(vow));
}
void CharacterEditor::AddShikigami(CurseUser& c, std::unique_ptr<int> shk){
    c.jujutsu.shikigami.push_back(std::move(shk));
}
void CharacterEditor::SetTechnique(CurseUser& c, std::unique_ptr<int> tech){
    c.jujutsu.technique = std::move(tech);
}
void CharacterEditor::SetDomain(CurseUser& c, std::unique_ptr<int> domain){
    c.jujutsu.domain = std::move(domain);
}
void CharacterEditor::SetDomainNullifier(CurseUser& c, std::unique_ptr<int> dnf){
    c.jujutsu.domain_neutralizer = std::move(dnf);
}
// traits
void CharacterEditor::SetTraitSixEyes(CurseUser& c, bool t){
    c.traits.six_eyes = t;
}
void CharacterEditor::SetTraitPassiveHealing(CurseUser& c, bool t){
    c.traits.passive_healing = t;
}
// sorcerer reverse cursed technique
void CharacterEditor::SetReverseCursedTechnique(Sorcerer& c, bool t){
    c.sorcerery.can_use_rct = t;
}
void CharacterEditor::SetReverseCursedTechniqueLevel(Sorcerer& c, Sorcerer::RCTLevel lvl){
    c.rct_level = lvl;
}