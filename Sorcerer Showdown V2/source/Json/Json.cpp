#include "../../header/Json.hpp"

DamageType JsonGet::GetDamageType(std::string_view sv){
    static const auto& m = UMaps::GetDTMap();
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    return DamageType::Normal;
}