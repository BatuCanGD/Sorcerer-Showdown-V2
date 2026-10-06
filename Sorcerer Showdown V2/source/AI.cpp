#include "../header/AI.hpp"
#include "../header/CharacterType/CurseUser.hpp"
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
    Technique* tech{nullptr};
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