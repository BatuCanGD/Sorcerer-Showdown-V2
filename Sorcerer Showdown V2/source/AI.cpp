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
                if (get_random<int>(0,77) >= 45 || !target){
                    target = c.get();
                }
            }
            break;
        }
    }
    if (!target){
        throw std::runtime_error("Only one person/Nobody left in battlefield vector, Impossible to find target");
    }
    return target;
}

AIConclusion AI::Fight(Character &user, Battlefield &bf, const FightingStyle fs, const ResourceUsage rs) {
    try {
        AI::GetTarget(user, user.Style().targeting_type, bf);
    } catch (const std::runtime_error& run){
        std::println("Runtime error: {}", run.what());
    }

    return {};
}