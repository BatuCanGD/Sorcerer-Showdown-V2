#include "../header/AI.hpp"
#include "../header/CharacterType/CurseUser.hpp"
#include "../header/Utilities/Random.hpp"
#include "../header/Battlefield.hpp"

#include <limits>
#include <print>
#include <stdexcept>

Character* AI::GetTarget(const Character& user, const TargetingType& tp, const Battlefield &bf){
    Character* target{nullptr};
    switch(tp){
        case TargetingType::HighestHP: {
            double highest_hp{std::numeric_limits<double>::lowest()};
            for (const auto& c : bf.battlefield){
                if (c.get() == &user) continue;
                if (c->Health() > highest_hp || !target){
                    target = c.get();
                    highest_hp = c->Health();
                }
            }
            break;
        }
        case TargetingType::LowestHP: {
            double lowest_hp{std::numeric_limits<double>::max()};
            for (const auto& c : bf.battlefield){
                if (c.get() == &user) continue;
                if (c->Health() < lowest_hp || !target){
                    target = c.get();
                    lowest_hp = c->Health();
                }
            }
            break;
        }
        case TargetingType::Mixed: {
            for (const auto& c : bf.battlefield){
                if (c.get() == &user) continue;
                if (get_random<int>(0,100) >= 50 || !target){
                    target = c.get();
                }
            }
            break;
        }
    }
    if (!target){
        throw std::runtime_error("0-1 Entities left in vector, Impossible to find target");
    }
    return target;
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