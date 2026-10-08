#include "../header/AI.hpp"
#include "../header/CharacterType/CurseUser.hpp"
#include "../header/Utilities/Random.hpp"
#include "../header/Battlefield.hpp"

#include <cmath>
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
            return bf.battlefield[get_random<size_t>(0, bf.battlefield.size() - 1)].get(); 
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
void AI::DoResourceManagement(Character& user, const Battlefield& bf ,const ResourceUsage& ru) { // possible shikigami, reinforcement and rct use
    [[maybe_unused]] Technique* tech{nullptr};
    if (const auto* c = user.CanUseSorcery()){
        if (auto t = c->Jujutsu().technique){
            tech = &*t;
        }
    }

    switch(ru){
        case ResourceUsage::AllOut: {

            break;
        }
        case ResourceUsage::Conservative: {

            break;
        }
        case ResourceUsage::Mixed: {

            break;
        }
        default:
            throw std::out_of_range("Unexpected Resource Usage");
    }
    return;
}




void AI::Fight(Character &user, Battlefield &bf) {
    const auto& style{user.Style()};

    try {
        DoResourceManagement(user, bf, style.resource_usage);
        Character* target = AI::GetTarget(user, style.targeting_type, bf);
        DoFighting(user, target, style);
    }catch(const std::out_of_range& oor) {
        std::println(stderr, "Range error: {}", oor.what());
    }catch(const std::runtime_error& re){
        std::println(stderr, "Runtime error: {}", re.what());
    }
}

void AI::SwitchWeapons(Character& c, WeaponPlacement which, WeaponPlacement where, WeaponChoice wc) {
    auto& inv = c.Equipment().inventory;
    const bool& has_inv = c.Equipment().has_access_to_inventory;
    auto& ct = c.Equipment().current_tool;
    auto& st = c.Equipment().stored_tool;
    CursedTool weapon{};
    switch(which){
        case AI::WeaponPlacement::Hand:
            if (!ct) return;
            weapon = std::move(*ct);
            ct.reset();
            break;
        case AI::WeaponPlacement::Offhand:
            if (!st) return;
            weapon = std::move(*st);
            st.reset();
            break;
        case AI::WeaponPlacement::Inventory: {
            if (!has_inv || inv.empty()){
                return;
            }
            CursedTool* erase_this{nullptr};
            switch(wc){
                case AI::WeaponChoice::EffectInducing:
                    for (auto& in : inv){
                        if (in.given_effect) {
                            weapon = std::move(in);
                            erase_this = &in;
                            break;
                        }
                    }
                    break;
                case AI::WeaponChoice::HighestDamage: {
                    CursedTool* best{nullptr};
                    for (auto& in : inv){
                        if (!best || in.damage > best->damage){
                            best = &in;
                        }
                    }
                    weapon = std::move(*best);
                    erase_this = best;
                    break;
                }
                case AI::WeaponChoice::TechniqueBypassing:
                    for (auto& in : inv){
                        if (in.damage_type == globalums::DamageType::BypassTech || in.damage_type == globalums::DamageType::BypassAll) {
                            weapon = std::move(in);
                            erase_this = &in;
                            break;
                        }
                    }
                    break;
                case AI::WeaponChoice::None:
                    auto& wep = c.Equipment().inventory[get_random<size_t>(0, c.Equipment().inventory.size() - 1)];
                    weapon = std::move(wep);
                    erase_this = &wep;
                    break;
                }
                std::erase_if(inv, [&](const auto& x){ 
                    return &x == erase_this;
                });
                break;
        }
    }
    switch(where){
        case AI::WeaponPlacement::Hand:
        case AI::WeaponPlacement::Offhand:
        case AI::WeaponPlacement::Inventory:
            if (!has_inv)
            break;
    }
}