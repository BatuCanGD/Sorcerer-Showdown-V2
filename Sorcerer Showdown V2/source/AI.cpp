#include "../header/AI.hpp"
#include "../header/CharacterType/CurseUser.hpp"
#include "../header/Systems/Mechanics/CursedToolSystem.hpp"
#include "../header/Systems/Mechanics/SorcerySystem.hpp"
#include "../header/Utilities/Random.hpp"
#include "../header/Battlefield.hpp"

#include <limits>
#include <print>
#include <stdexcept>

Character* AI::GetTarget(const Character& user, const TargetingType& tp, const Battlefield &bf){
    if (bf.battlefield.size() <= 1){
        throw std::runtime_error("0-1 Entities left in vector, Impossible to find target");
    }
    switch(tp){
        case TargetingType::HighestHP: {
            Character* target{nullptr};
            double highest_hp{std::numeric_limits<double>::lowest()};
            for (const auto& c : bf.battlefield){
                if (c.get() == &user || c->State().health <= 0.0) continue;
                if (c->State().health > highest_hp || !target){
                    target = c.get();
                    highest_hp = c->State().health;
                }
            }
            return target;
        }
        case TargetingType::LowestHP: {
            Character* target{nullptr};
            double lowest_hp{std::numeric_limits<double>::max()};
            for (const auto& c : bf.battlefield){
                if (c.get() == &user || c->State().health <= 0.0) continue;
                if (c->State().health < lowest_hp || !target){
                    target = c.get();
                    lowest_hp = c->State().health;
                }
            }
            return target;
        }
        case TargetingType::Mixed: {
            Character* target{nullptr};
            do {
                target = bf.battlefield[get_random<size_t>(0, bf.battlefield.size() - 1)].get();
            }while(target->State().health <= 0.0 || target == &user);
            return target;
        }
    }
    return nullptr;
}

void AI::DoFighting(Character& user, Character* target, const BattleIQ& fs) { // possible technique/domain use
    if (!target){
        throw std::runtime_error("No target exists");
    }
    switch(fs.fighting_style){
        case FightingStyle::Aggressive: {
            if ([[maybe_unused]]const auto& c = user.CanUseSorcery()){
                [[maybe_unused]]const bool use_tech = fs.resource_usage != ResourceUsage::Conservative && c->Output().status != CurseUserOutput::Status::BurntOut && !(c->Output().current_output * 1.25 > c->Output().max_output_potential);
                
            }
            
            //SwitchWeapons(user.Equipment(), (user.Equipment().has_access_to_inventory && !user.Equipment().inventory.empty()) ? WeaponPlacement::Inventory : WeaponPlacement::Offhand, WeaponPlacement::Hand, WeaponChoice::HighestDamage);
            break;
        }
        case FightingStyle::Defensive: {

            break;
        }
        case FightingStyle::Mixed: {

            break;
        }
        default:
            throw std::out_of_range("Unexpected Fighting Style");
    }
    return;
}
void AI::DoResourceManagement(Character& user, const Character& tr,const Battlefield& bf ,const ResourceUsage& ru) {
    auto* c = user.CanUseSorcery();

    AI::RM_UseTools(user, tr);
    if (c){
        AI::RM_UseReinforcement(*c, ru);
        AI::RM_UseShikigami(*c, ru);
        AI::RM_UseRCT(*c, ru);
    }
}

void AI::RM_UseShikigami(CurseUser &c, const ResourceUsage &rs){
    if (c.Jujutsu().shikigami.empty()) return;
    SummonType type{};
    switch(rs){
        case ResourceUsage::AllOut: {     
            if (SorcerySystem::CEMoreThanMx(c, 0.45)){
                type = SummonType::Active;  
            }else if (SorcerySystem::CEMoreThanMx(c, 0.15)){
                type = SummonType::Support;
            }else{
                type = SummonType::Shadow;
            }
            break;
        }
        case ResourceUsage::Mixed: {
            if (SorcerySystem::CEMoreThanMx(c, get_random<double>(0.30, 0.60))){
                type = SummonType::Active;  
            }else if (SorcerySystem::CEMoreThanMx(c, get_random<double>(0.05, 0.30))){
                type = SummonType::Support;
            }else{
                type = SummonType::Shadow;
            }
            break;
        }
        case ResourceUsage::Conservative: {
            if (SorcerySystem::CEMoreThanMx(c, 0.65)){
                type = SummonType::Active;  
            }else if (SorcerySystem::CEMoreThanMx(c, 0.40)){
                type = SummonType::Support;
            }else{
                type = SummonType::Shadow;
            }
            break;
        }
    }
    for(auto& s : c.Jujutsu().shikigami){
        s.summon_type = type;
    }
}
void AI::RM_UseReinforcement(CurseUser &c, const ResourceUsage &rs) {
    double val{0.0};
    switch(rs){
        case ResourceUsage::AllOut:

            break;
        case ResourceUsage::Mixed:

            break;
        case ResourceUsage::Conservative:

            break;
    }
    c.CursedEnergy().reinforcement_amount = val;
}
void AI::RM_UseRCT(CurseUser &c, const ResourceUsage &rs) {
    if (!c.RCT().can_use_rct){
        return;
    }
    double val{0.0};
    switch(rs){
        case ResourceUsage::AllOut:

            break;
        case ResourceUsage::Mixed:

            break;
        case ResourceUsage::Conservative:

            break;
    }
    c.RCT().rct_output = val;
}
void AI::RM_UseTools(Character &c, const Character & tr) {
    auto& eq = c.Equipment();
    if ((!eq.current_tool && !eq.stored_tool && (!eq.has_access_to_inventory || eq.inventory.empty()))){
        return;
    }
    if (const auto& cr = tr.CanUseSorcery()){
        if (const auto& t = cr->Jujutsu().technique) {
            if (t->HasBarrier()){
                if (CursedToolSystem::DoesBypassTech(*eq.current_tool)) {
                    return;
                }
                if (CursedToolSystem::DoesBypassTech(*eq.stored_tool)) {

                }
                
            }
        }
    }
}


void AI::Fight(Character &user, Battlefield &bf) {
    const auto& style{user.Style()};

    try {
        Character* target = AI::GetTarget(user, style.targeting_type, bf);
        DoResourceManagement(user, *target, bf, style.resource_usage);
        DoFighting(user, target, style);
    }catch(const std::out_of_range& oor) {
        std::println(stderr, "Range error: {}", oor.what());
    }catch(const std::runtime_error& re){
        std::println(stderr, "Runtime error: {}", re.what());
    }
}

CursedTool AI::GetFromInv(CharInv& eq, WeaponChoice wc) {
    auto& inv = eq.inventory;
    if (inv.empty()) {
        throw std::runtime_error("No inventory items to choose");
    }

    size_t chosen = inv.size();
    switch (wc) {
        case WeaponChoice::EffectInducing:
            for (size_t i = 0; i < inv.size(); ++i) {
                if (chosen == inv.size() || inv[i].given_effect) {
                    chosen = i;
                    break;
                }
            }
            break;
        case WeaponChoice::HighestDamage:
            chosen = 0;
            for (size_t i = 1; i < inv.size(); ++i) {
                if (chosen == inv.size() || inv[i].damage > inv[chosen].damage) {
                    chosen = i;
                }
            }
            break;
        case WeaponChoice::TechniqueBypassing:
            for (size_t i = 0; i < inv.size(); ++i) {
                if (chosen == inv.size() || inv[i].damage_type == globalums::DamageType::BypassTech || inv[i].damage_type == globalums::DamageType::BypassAll) {
                    chosen = i;
                    break;
                }
            }
            break;
        case WeaponChoice::None:
            chosen = get_random<size_t>(0, inv.size() - 1);
            break;
        default:
            throw std::runtime_error("Invalid weapon choice");
    }
    CursedTool selected = std::move(inv[chosen]);
    inv.erase(inv.begin() + chosen);
    return selected;
}

CursedTool AI::GetWeapon(CharInv& eq, WeaponPlacement which, WeaponChoice which_type) {
    switch(which){
        case AI::WeaponPlacement::Hand:
            if (eq.current_tool) {
                CursedTool selected = std::move(*eq.current_tool);
                eq.current_tool.reset();
                return selected;
            }
            break;
        case AI::WeaponPlacement::Offhand:
            if (eq.stored_tool){
                CursedTool selected = std::move(*eq.stored_tool);
                eq.stored_tool.reset();
                return selected;
            }
            break;
        case AI::WeaponPlacement::Inventory:
            return AI::GetFromInv(eq, which_type);
        default:
            break;
    }
    throw std::runtime_error("No Weapon able to be chosen");
}
bool AI::HasWeapon(CharInv &c, WeaponChoice wc){
    switch(wc){
        case AI::WeaponChoice::EffectInducing:

            break;
        case AI::WeaponChoice::TechniqueBypassing: {
            if (CursedToolSystem::DoesBypassTech(*c.current_tool) || CursedToolSystem::DoesBypassTech(*c.stored_tool)) {
                return true;
            }
            if (c.has_access_to_inventory){
                for (const auto& cc : c.inventory){
                    if (CursedToolSystem::DoesBypassTech(cc)) return true;
                }
            }
            break;
        }
        default: 
            return true;
    }
    return false;
}
void AI::MoveWeapon(CharInv& c, CursedTool wp, WeaponPlacement where) {
    const bool& has_inv = c.has_access_to_inventory;
    auto& inv = c.inventory;
    auto& cur = c.current_tool;
    auto& stor = c.stored_tool;

    switch(where){
        case AI::WeaponPlacement::Hand: {
            if (cur){
                auto temp = std::move(cur);
                cur.emplace(std::move(wp));
                if (has_inv){
                    inv.push_back(std::move(*temp));
                }else {
                    stor.emplace(std::move(*temp));
                }
            }else{
                cur.emplace(std::move(wp));
            }
            break;
        }
        case AI::WeaponPlacement::Offhand: {
            if (stor){
                auto temp = std::move(stor);
                stor.emplace(std::move(wp));
                if (has_inv){
                    inv.push_back(std::move(*temp));
                }else{
                    cur.emplace(std::move(*temp));
                }
            }else{
                stor.emplace(std::move(wp));
            }
            break;
        }
        case AI::WeaponPlacement::Inventory:
            if (has_inv){
                inv.push_back(std::move(wp));
            }else{
                throw std::runtime_error("Cannot put weapon in inventory without access to inventory");
            }
            break;
    }
}

void AI::SwitchWeapons(CharInv& eq, WeaponPlacement which, WeaponPlacement where, WeaponChoice wc) {
    AI::MoveWeapon(eq, AI::GetWeapon(eq, which, wc), where);
}