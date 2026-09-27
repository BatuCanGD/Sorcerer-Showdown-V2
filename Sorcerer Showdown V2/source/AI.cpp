#include "../header/AI.hpp"
#include "../header/Utilities/Random.hpp"
#include "../header/Battlefield.hpp"

#include <limits>
#include <print>
#include <stdexcept>

Character* AI::GetTarget(const Character& user, const TargetingType tp, const Battlefield &bf){
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

void AI::DoFightingStyle(Character& user, Character* target, const FightingStyle fs) { // possible technique/domain use
    if (!target){
        throw std::runtime_error("No target exists");
    }
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
        default:
            throw std::out_of_range("Unexpected Fighting Style");
    }
    return;
}
void AI::DoResourceUsage(Character& user, const Battlefield& bf ,const ResourceUsage ru) { // possible shikigami, reinforcement and rct use
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
        DoResourceUsage(user, bf, style.resource_usage);
    }catch(const std::out_of_range& rn) {
        std::println(stderr, "Range error: {}", rn.what());
    }

    Character* target{nullptr};

    try {
        target = AI::GetTarget(user, style.targeting_type, bf);
    } catch (const std::runtime_error& run){
        std::println(stderr, "Runtime error: {}", run.what());
    }

    try {
        DoFightingStyle(user, target, style.fighting_style);
    }catch(const std::out_of_range& rn) {
        std::println(stderr, "Range error: {}", rn.what());
    }catch(const std::runtime_error& rn){
        std::println(stderr, "Runtime error: {}", rn.what());
    }
}