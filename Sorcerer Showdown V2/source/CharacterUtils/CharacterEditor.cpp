#include "../../header/CharacterUtils/CharacterEditor.hpp"
#include "../../header/CharacterType/Character.hpp"
#include "../../header/CharacterType/CurseUser.hpp"
#include "../../header/CharacterType/Sorcerer.hpp"

#include <print>
#include <utility>
#include <string_view>

// identity
void Editor::SetName(Character& c, std::string_view name){
    c.identity.name = name;
}
void Editor::SetColor(Character& c, std::string_view color){
    c.identity.color = color;
}
// base state
void Editor::SetHealth(Character& c, double hp){
    c.state.max_health = hp;
    c.state.health = hp;
    c.state.previous_health = hp;
}
void Editor::SetInvulnerability(Character& c, bool t){
    c.state.is_invulnerable = t;
}
void Editor::SetDurability(Character &c, double dr){
    c.state.durability = dr;
}
void Editor::SetStrength(Character &c, double str){
    c.state.strength = str;
}
// inventory
void Editor::GiveCharacterTool(Character& c, std::unique_ptr<int> tool, ItemPlacement place){
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
void Editor::SetInventoryAccess(Character& c, bool t){
    c.equipment.has_access_to_inventory = t;
}
// inside curseuser
void Editor::SetCursedEnergyEfficiency(CurseUser& c, CurseUser::CEfficiency efficiency){
    c.ce_efficiency = efficiency;
}
// curse user system
void Editor::SetCursedEnergy(CurseUser &c, double ce){
    c.sorcery.max_cursed_energy = ce;
    c.sorcery.cursed_energy = ce;
    c.sorcery.previous_cursed_energy = ce;
}
void Editor::AddBindingVow(CurseUser& c, std::unique_ptr<int> vow){
    c.jujutsu.binding_vows.push_back(std::move(vow));
}
void Editor::AddShikigami(CurseUser& c, std::unique_ptr<int> shk){
    c.jujutsu.shikigami.push_back(std::move(shk));
}
void Editor::SetTechnique(CurseUser& c, std::unique_ptr<int> tech){
    c.jujutsu.technique = std::move(tech);
}
void Editor::SetDomain(CurseUser& c, std::unique_ptr<int> domain){
    c.jujutsu.domain = std::move(domain);
}
void Editor::SetDomainNullifier(CurseUser& c, std::unique_ptr<int> dnf){
    c.jujutsu.domain_neutralizer = std::move(dnf);
}
// traits
void Editor::SetTraitSixEyes(CurseUser& c, bool t){
    c.traits.six_eyes = t;
}
void Editor::SetTraitPassiveHealing(CurseUser& c, bool t){
    c.traits.passive_healing = t;
}
// sorcerer reverse cursed technique
void Editor::SetReverseCursedTechnique(Sorcerer& c, bool t){
    c.sorcerery.can_use_rct = t;
}
void Editor::SetReverseCursedTechniqueLevel(Sorcerer& c, Sorcerer::RCTLevel lvl){
    c.rct_level = lvl;
}