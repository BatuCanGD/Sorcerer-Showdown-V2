#include "../../../header/Systems/Mechanics/CursedToolSystem.hpp"
#include "../../../header/DamageType.hpp"
#include "../../../header/General/CursedTool.hpp"
#include "../../../header/Utilities/Random.hpp"
#include <stdexcept>

void CursedToolSystem::SwitchTo(Base::Equipment &e, WeaponChoice wc) {
    const bool& has_inv = e.has_access_to_inventory;    
    switch (wc){
        case CursedToolSystem::WeaponChoice::EffectInducing:
            if (e.current_tool && e.current_tool->given_effect) {
                break;
            }
            else if (e.stored_tool && e.stored_tool->given_effect){
                SwitchWeapons(e, WeaponPlacement::Offhand, WeaponPlacement::Hand);
            }
            else if (has_inv){
                for (const auto& c : e.inventory){
                    if (c.given_effect){
                        SwitchWeapons(e, WeaponPlacement::Inventory, WeaponPlacement::Hand, WeaponChoice::EffectInducing);
                        break;
                    }
                }
            }
            break;
        case CursedToolSystem::WeaponChoice::HighestDamage:
            if (has_inv){
                SwitchWeapons(e, WeaponPlacement::Inventory, WeaponPlacement::Hand, WeaponChoice::HighestDamage);
            }else{
                if (e.current_tool && e.stored_tool){
                    if (e.current_tool->damage > e.stored_tool->damage){
                        break;
                    }else{
                        SwitchWeapons(e, WeaponPlacement::Offhand, WeaponPlacement::Hand);
                    }
                }else if (e.stored_tool){
                    SwitchWeapons(e, WeaponPlacement::Offhand, WeaponPlacement::Hand);
                }
            }
            break;
        case CursedToolSystem::WeaponChoice::TechniqueBypassing:
            if (e.current_tool && DamageCheck::BypassesTech(e.current_tool->damage_type)) {
                break;
            }
            else if (e.stored_tool && DamageCheck::BypassesTech(e.stored_tool->damage_type)){
                SwitchWeapons(e, WeaponPlacement::Offhand, WeaponPlacement::Hand);
            }
            else if (has_inv){
                for (const auto& c : e.inventory){
                    if (DamageCheck::BypassesTech(c.damage_type)){
                        SwitchWeapons(e, WeaponPlacement::Inventory, WeaponPlacement::Hand, WeaponChoice::TechniqueBypassing);
                        break;
                    }
                }
            }
            break;
        case CursedToolSystem::WeaponChoice::Random:
            SwitchWeapons(e, has_inv ? WeaponPlacement::Inventory : WeaponPlacement::Offhand, WeaponPlacement::Hand);
            break;
        case CursedToolSystem::WeaponChoice::Unequip:
            if (e.current_tool){
                SwitchWeapons(e, WeaponPlacement::Hand, has_inv ? WeaponPlacement::Inventory : WeaponPlacement::Offhand);
            }
            break;
        default: [[fallthrough]]; 
        case CursedToolSystem::WeaponChoice::None:
            break;
    }
}

CursedTool CursedToolSystem::GetFromInv(Base::Equipment& eq, WeaponChoice wc) {
    auto& inv = eq.inventory;
    if (inv.empty() || wc == WeaponChoice::None) {
        return {};
    }

    size_t chosen = inv.size() - 1;
    switch (wc) {
        case WeaponChoice::EffectInducing:
            for (size_t i = 0; i < inv.size(); ++i) {
                if (inv[i].given_effect) {
                    chosen = i;
                    break;
                }
            }
            break;
        case WeaponChoice::HighestDamage:
            chosen = 0;
            for (size_t i = 1; i < inv.size(); ++i) {
                if (inv[i].damage > inv[chosen].damage) {
                    chosen = i;
                }
            }
            break;
        case WeaponChoice::TechniqueBypassing:
            for (size_t i = 0; i < inv.size(); ++i) {
                if (inv[i].damage_type == DamageType::BypassTech || inv[i].damage_type == DamageType::BypassAll) {
                    chosen = i;
                    break;
                }
            }
            break;
        case WeaponChoice::Random:
            chosen = get_random<size_t>(0, inv.size() - 1);
            break;
        default:
            throw std::runtime_error("Invalid weapon choice");
    }
    CursedTool selected = std::move(inv[chosen]);
    inv.erase(inv.begin() + chosen);
    return selected;
}

CursedTool CursedToolSystem::GetWeapon(Base::Equipment& eq, WeaponPlacement which, WeaponChoice which_type) {
    if (which_type == WeaponChoice::Unequip){
        return {};
    }
    switch(which){
        case CursedToolSystem::WeaponPlacement::Hand:
            if (eq.current_tool) {
                CursedTool selected = std::move(*eq.current_tool);
                eq.current_tool.reset();
                return selected;
            }
            break;
        case CursedToolSystem::WeaponPlacement::Offhand:
            if (eq.stored_tool){
                CursedTool selected = std::move(*eq.stored_tool);
                eq.stored_tool.reset();
                return selected;
            }
            break;
        case CursedToolSystem::WeaponPlacement::Inventory:
            return CursedToolSystem::GetFromInv(eq, which_type);
        default:
            break;
    }
    throw std::runtime_error("No Weapon able to be chosen");
}
bool CursedToolSystem::HasWeapon(Base::Equipment &c, WeaponChoice wc){
    switch(wc){
        case CursedToolSystem::WeaponChoice::EffectInducing:
            if ((c.current_tool && c.current_tool->given_effect) || (c.stored_tool && c.stored_tool->given_effect)){
                return true;
            }
            if (c.has_access_to_inventory){
                for (const auto& cc : c.inventory){
                    if (cc.given_effect) return true;
                }
            }
            break;
        case CursedToolSystem::WeaponChoice::TechniqueBypassing: {
            if ((c.current_tool && DamageCheck::BypassesTech(c.current_tool->damage_type)) || (c.stored_tool && DamageCheck::BypassesTech(c.stored_tool->damage_type))) {
                return true;
            }
            if (c.has_access_to_inventory){
                for (const auto& cc : c.inventory){
                    if (DamageCheck::BypassesTech(cc.damage_type)) return true;
                }
            }
            break;
        }
        default: 
            return true;
    }
    return false;
}
void CursedToolSystem::MoveWeapon(Base::Equipment& c, CursedTool wp, WeaponPlacement where, WeaponChoice wc) {
    const bool unequip = wc == WeaponChoice::Unequip;
    const bool& has_inv = c.has_access_to_inventory;
    auto& inv = c.inventory;
    auto& cur = c.current_tool;
    auto& stor = c.stored_tool;

    switch(where){
        case CursedToolSystem::WeaponPlacement::Hand: {
            if (cur){
                CursedTool temp = std::move(*cur);
                if (unequip) cur.reset();
                else cur.emplace(std::move(wp));
                if (has_inv){
                    inv.push_back(std::move(temp));
                }else {
                    stor.emplace(std::move(temp));
                }
            }else if (!unequip){
                cur.emplace(std::move(wp));
            }
            break;
        }
        case CursedToolSystem::WeaponPlacement::Offhand: {
            if (stor){
                CursedTool temp = std::move(*stor);
                if (unequip) stor.reset();
                else stor.emplace(std::move(wp));
                
                if (has_inv){
                    inv.push_back(std::move(temp));
                }else{
                    cur.emplace(std::move(temp));
                }
            }else if (!unequip){
                stor.emplace(std::move(wp));
            }
            break;
        }
        case CursedToolSystem::WeaponPlacement::Inventory:
            if (unequip){
                return;
            }
            if (has_inv){
                inv.push_back(std::move(wp));
            }else{
                throw std::runtime_error("Cannot put weapon in inventory without access to inventory");
            }
            break;
    }
}

void CursedToolSystem::SwitchWeapons(Base::Equipment& eq, WeaponPlacement which, WeaponPlacement where, WeaponChoice wc) {
    CursedToolSystem::MoveWeapon(eq, CursedToolSystem::GetWeapon(eq, which, wc), where, wc);
}