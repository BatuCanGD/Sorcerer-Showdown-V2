#include "../../header/Json.hpp"

DamageType JsonGet::GetDamageType(std::string_view sv){
    static const UMap<DamageType> m {
        {"Bypass All", DamageType::BypassAll},
        {"Bypass Reinforcement", DamageType::BypassRein},
        {"Bypass Techniques", DamageType::BypassTech},
    };
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    return DamageType::Normal;
}