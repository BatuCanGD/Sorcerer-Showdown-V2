#include "../header/AI.hpp"
#include "../header/Logger.hpp"
#include "../header/Systems/Mechanics/CombatSystem.hpp"
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

void AI::DoFighting(Character& user, Character* target, const Battlefield& bf, const BattleIQ& iq) { // possible technique/domain use
    if (!target){
        throw std::runtime_error("No target exists");
    }
    if (auto* c = user.CanUseSorcery()){
        AI::DF_UseDomain(*c, bf, iq.fighting_style);
        if (AI::DF_UseTechnique(*c, *target, iq)) return;
    }
    const auto l = user.Attack(*target);
    Log::Attack(l, user, *target);
}
void AI::DoResourceManagement(Character& user, const Character& tr,const Battlefield& bf ,const ResourceUsage& ru) {
    AI::DRM_UseTools(user, tr);
    if (auto* c = user.CanUseSorcery()){
        AI::DRM_UseReinforcement(*c, ru);
        AI::DRM_UseShikigami(*c, ru);
        AI::DRM_UseBindingVow(*c, tr, bf, ru);
        AI::DRM_UseRCT(*c, ru);
    }
}

void AI::DF_UseDomain(CurseUser &c, const Battlefield &bf, const FightingStyle& fs) {
    size_t domain_users{0};
    for (const auto& cc : bf.battlefield){
        if (const auto* cr = cc->CanUseSorcery()) {
            if (cr->Jujutsu().domain && cr->Jujutsu().domain->is_active) domain_users++;
        }
    }
    const double domain_cost = c.Jujutsu().domain ? SorcerySystem::ApplySpendingMultiplier(c.Jujutsu().domain->cost, c) : 0.0;
    const bool can_use_domain = c.Jujutsu().domain && c.Output().status != CurseUserOutput::Status::BurntOut && c.CursedEnergy().cursed_energy >= domain_cost;
    const bool should_use_domain = can_use_domain && domain_users == 1;
    const bool needs_neutralizer = c.Jujutsu().neutralizer && domain_users > 1 && (c.Output().status == CurseUserOutput::Status::BurntOut || c.CursedEnergy().cursed_energy < domain_cost);

    switch(fs){
        case FightingStyle::Aggressive: {


            break;
        }
        case FightingStyle::Defensive: {
        
            break;
        }
        case FightingStyle::Mixed: {

            break;
        }
    }


    

}
bool AI::DF_UseTechnique(CurseUser &c, Character &target, const BattleIQ &iq) {





    return false;
}
void AI::DRM_UseShikigami(CurseUser &c, const ResourceUsage &rs){
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
void AI::DRM_UseReinforcement(CurseUser &c, const ResourceUsage &rs) {
    double val{0.0};
    switch(rs){
        case ResourceUsage::AllOut:
            if (SorcerySystem::CEMoreThanMx(c, 0.40) && c.State().health < c.State().max_health * 0.40){
                val = 135.0;
            }else if (SorcerySystem::CEMoreThanMx(c, 0.20) && c.State().health < c.State().max_health * 0.33){
                val = 90.0;
            }else if (c.State().health < c.State().max_health * 0.25){
                val = 60.0;
            }
            break;
        case ResourceUsage::Mixed:
            if (SorcerySystem::CEMoreThanMx(c, get_random<double>(0.25, 0.75)) && c.State().health < c.State().max_health * get_random<double>(0.60, 0.90)){
                val = get_random<double>(70.0, 150.0);
            }else if (SorcerySystem::CEMoreThanMx(c, 0.20) && c.State().health < c.State().max_health * get_random<double>(0.30, 0.60)){
                val = get_random<double>(65.0, 100.0);
            }else if (c.State().health < c.State().max_health * get_random<double>(0.01, 0.30)){
                val = get_random<double>(20.0, 80.0);
            }
            break;
        case ResourceUsage::Conservative:
            if (SorcerySystem::CEMoreThanMx(c, 0.60) && c.State().health < c.State().max_health * 0.55){
                val = 90.0;
            }else if (SorcerySystem::CEMoreThanMx(c, 0.40) && c.State().health < c.State().max_health * 0.35){
                val = 50.0;
            }else if (c.State().health < c.State().max_health * 0.30){
                val = 35.0;
            }
            break;
    }
    c.CursedEnergy().reinforcement_amount = val;
}
void AI::DRM_UseRCT(CurseUser &c, const ResourceUsage &rs) {
    if (!c.RCT().can_use_rct){
        return;
    }
    double val{0.0};
    switch(rs){
        case ResourceUsage::AllOut:
            if (SorcerySystem::CEMoreThanMx(c, 0.40) && c.State().health < c.State().max_health * 0.40){
                val = 115.0;
            }else if (SorcerySystem::CEMoreThanMx(c, 0.20) && c.State().health < c.State().max_health * 0.33){
                val = 85.0;
            }else if (c.State().health < c.State().max_health * 0.25){
                val = 65.0;
            }
            break;
        case ResourceUsage::Mixed:
            if (SorcerySystem::CEMoreThanMx(c, get_random<double>(0.25, 0.75)) && c.State().health < c.State().max_health * get_random<double>(0.60, 0.90)){
                val = get_random<double>(50.0, 150.0);
            }else if (SorcerySystem::CEMoreThanMx(c, 0.20) && c.State().health < c.State().max_health * get_random<double>(0.30, 0.60)){
                val = get_random<double>(35.0, 80.0);
            }else if (c.State().health < c.State().max_health * get_random<double>(0.01, 0.30)){
                val = get_random<double>(10.0, 50.0);
            }
            break;
        case ResourceUsage::Conservative:
            if (SorcerySystem::CEMoreThanMx(c, 0.60) && c.State().health < c.State().max_health * 0.55){
                val = 100.0;
            }else if (SorcerySystem::CEMoreThanMx(c, 0.40) && c.State().health < c.State().max_health * 0.35){
                val = 55.0;
            }else if (c.State().health < c.State().max_health * 0.30){
                val = 50.0;
            }
            break;
    }
    c.RCT().rct_output = val;
}
void AI::DRM_UseBindingVow(CurseUser &c, const Character &tr, const Battlefield &bf, const ResourceUsage &iq) {

}
void AI::DRM_UseTools(Character &c, const Character & tr) {
    auto& eq = c.Equipment();
    if ((!eq.current_tool && !eq.stored_tool && (!eq.has_access_to_inventory || eq.inventory.empty()))){
        return;
    }
    if (const auto& cr = tr.CanUseSorcery()){
        if (const auto& t = cr->Jujutsu().technique) {
            if (t->HasBarrier()){
                AI::SwitchTo(eq, WeaponChoice::TechniqueBypassing);
                return;
            }
        }
    }
    if (tr.State().health > c.State().health || tr.State().health > tr.State().max_health * 0.7){
        SwitchTo(eq, WeaponChoice::HighestDamage);
    }
}

void AI::SwitchTo(CharInv &eq, WeaponChoice wc) {
    const bool& has_inv = eq.has_access_to_inventory;    
    if (wc == WeaponChoice::EffectInducing || wc == WeaponChoice::TechniqueBypassing){
        auto is_the_one = [wc](const CursedTool* ct)->bool{
            return ct && ((wc == WeaponChoice::EffectInducing && ct->given_effect) || (wc == WeaponChoice::TechniqueBypassing && CursedToolSystem::DoesBypassTech(*ct)));
        };
        if (is_the_one(&*eq.current_tool)){
            return;
        }else if (wc == WeaponChoice::Unequip && eq.current_tool){
            AI::SwitchWeapons(eq, WeaponPlacement::Hand, eq.has_access_to_inventory ? WeaponPlacement::Inventory : WeaponPlacement::Offhand);
            return;
        }
        if (is_the_one(&*eq.stored_tool)) {
            AI::SwitchWeapons(eq, WeaponPlacement::Offhand, WeaponPlacement::Hand);
            return;
        }
        if (has_inv){
            for (const auto& r : eq.inventory){
                if (is_the_one(&r)){
                    AI::SwitchWeapons(eq, WeaponPlacement::Inventory, WeaponPlacement::Hand, wc);
                    return;
                }
            }
        }
    }else if (wc == WeaponChoice::HighestDamage) {
        if (!has_inv){
            AI::SwitchWeapons(eq, WeaponPlacement::Offhand, WeaponPlacement::Hand, WeaponChoice::HighestDamage);
        }else{
            AI::SwitchWeapons(eq, !eq.inventory.empty() ? WeaponPlacement::Inventory : WeaponPlacement::Offhand, WeaponPlacement::Hand, WeaponChoice::HighestDamage);
        }
    }else{
        AI::SwitchWeapons(eq, WeaponPlacement::Hand, has_inv ? WeaponPlacement::Inventory : WeaponPlacement::Offhand , WeaponChoice::None); 
    }
}


void AI::Fight(Character &user, Battlefield &bf) {
    const auto& style{user.Style()};

    try {
        Character* target = AI::GetTarget(user, style.targeting_type, bf);
        DoResourceManagement(user, *target, bf, style.resource_usage);
        DoFighting(user, target, bf, style);
    }catch(const std::out_of_range& oor) {
        std::println(stderr, "Range error: {}", oor.what());
    }catch(const std::runtime_error& re){
        std::println(stderr, "Runtime error: {}", re.what());
    }
}

CursedTool AI::GetFromInv(CharInv& eq, WeaponChoice wc) {
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
                if (inv[i].damage_type == globalums::DamageType::BypassTech || inv[i].damage_type == globalums::DamageType::BypassAll) {
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

CursedTool AI::GetWeapon(CharInv& eq, WeaponPlacement which, WeaponChoice which_type) {
    if (which_type == WeaponChoice::Unequip){
        return {};
    }
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
            if ((c.current_tool && c.current_tool->given_effect) || (c.stored_tool && c.stored_tool->given_effect)){
                return true;
            }
            if (c.has_access_to_inventory){
                for (const auto& cc : c.inventory){
                    if (cc.given_effect) return true;
                }
            }
            break;
        case AI::WeaponChoice::TechniqueBypassing: {
            if ((c.current_tool && CursedToolSystem::DoesBypassTech(*c.current_tool)) || (c.stored_tool && CursedToolSystem::DoesBypassTech(*c.stored_tool))) {
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
void AI::MoveWeapon(CharInv& c, CursedTool wp, WeaponPlacement where, WeaponChoice wc) {
    const bool unequip = wc == WeaponChoice::Unequip;
    const bool& has_inv = c.has_access_to_inventory;
    auto& inv = c.inventory;
    auto& cur = c.current_tool;
    auto& stor = c.stored_tool;

    switch(where){
        case AI::WeaponPlacement::Hand: {
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
        case AI::WeaponPlacement::Offhand: {
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
        case AI::WeaponPlacement::Inventory:
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

void AI::SwitchWeapons(CharInv& eq, WeaponPlacement which, WeaponPlacement where, WeaponChoice wc) {
    AI::MoveWeapon(eq, AI::GetWeapon(eq, which, wc), where, wc);
}