#include "../../header/Json.hpp"

void JsonModify::ModifyShikigami(const nlohmann::json &j, Shikigami& c){
    if (j.contains("Identity")){
        const auto& id = j.at("Identity");
        c.id.name = Json::GetValue<std::string>(id, "Name", "Delamain");
        c.id.color = Json::GetValue<std::string>(id, "Color", "\x1b[38;5;117m");
        c.id.description = Json::GetValue<std::string>(id, "Description");
    }
    if (j.contains("Stats")) {
        const auto& st = j.at("Stats");
        c.stats.max_health = Json::GetValue<double>(st, "Health", 100.0);
        c.stats.health = c.stats.max_health;
        c.stats.durability = Json::GetValue<double>(st, "Durability", 10.0);
        c.stats.regen_speed = Json::GetValue<double>(st, "Health Regeneration Speed", 15.0);
        c.cost = Json::GetValue<double>(st, "Cost", 100.0);
        c.support_type = JsonGet::GetSupportType(Json::GetValue<std::string_view>(st, "Support Type"));
        c.active_ability = JsonGet::GetActiveAbility(Json::GetValue<std::string_view>(st, "Activation Ability"));
    }
}

ActiveAbility JsonGet::GetActiveAbility(std::string_view sv) {
    static const UMap<ActiveAbility> m {
        {"Technique Bypassing", ActiveAbility::TechniqueBypass}
    };
    for (const auto& c : m){
        if (c.first == sv){
            return c.second;
        }
    }
    return ActiveAbility::None;
}
SupportType JsonGet::GetSupportType(std::string_view sv) {
    static const UMap<SupportType> m {
        {"Strength", SupportType::Offense},
        {"Durability", SupportType::Defense},
        {"Cursed Energy Regeneration", SupportType::CursedEnergyRegen},
        {"Output", SupportType::Output}
    };
    for (const auto& c : m){
        if (c.first == sv){
            return c.second;
        }
    }
    return SupportType::None;
}