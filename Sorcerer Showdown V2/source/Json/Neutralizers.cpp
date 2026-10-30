#include "../../header/Json.hpp"


void JsonModify::ModifyNeutralizer(const nlohmann::json &j, Neutralizer& c){
    if (j.contains("Identity")){
        const auto& id = j.at("Identity");
        c.identity.name = Json::GetValue<std::string>(id, "Name", "Delamain");
        c.identity.color = Json::GetValue<std::string>(id, "Color", "\x1b[38;5;117m");
        c.identity.description = Json::GetValue<std::string>(id, "Description");
    }
    if (j.contains("Stats")) {
        const auto& st = j.at("Stats");
        c.max_health = Json::GetValue<double>(st, "Health", 50.0);
        c.health = c.max_health;
        c.durability = Json::GetValue<double>(st, "Durability", 50.0);
        c.cost = Json::GetValue<double>(st, "Cost", 50.0);
        c.neutralizer_type = Json::GetValue<std::string_view>(st, "Type") == "Damage Reducing" ? NeutralizerType::ReducedDamage : NeutralizerType::FullyProtected;
    }
}

std::optional<Neutralizer> JsonGet::GetNeutralizer(std::string_view sv) {
    static const auto& m = UMaps::GetNLMap();
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    for (const auto& c : Json::LoadEntity<Neutralizer>()){
        if (c.identity.name == sv) return c;
    }
    return std::nullopt;
}
