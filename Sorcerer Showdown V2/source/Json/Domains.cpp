#include "../../header/Json.hpp"

void JsonModify::ModifyDomain(const nlohmann::json &j, Domain& c){
    if (j.contains("Identity")){
        const auto& id = j.at("Identity");
        c.identity.name = Json::GetValue<std::string>(id, "Name", "Delamain");
        c.identity.color = Json::GetValue<std::string>(id, "Color", "\x1b[38;5;117m");
        c.identity.description = Json::GetValue<std::string>(id, "Description");
    }
    c.type = Json::GetValue<std::string_view>(j, "Type") == "Open" ? DomainType::Open : DomainType::Closed;
    if (j.contains("Stats")) {
        const auto& st = j.at("Stats");
        c.max_health = Json::GetValue<double>(st, "Health", 50.0);
        c.health = c.max_health;
        c.durability = Json::GetValue<double>(st, "Durability", 50.0);
        c.damage = Json::GetValue<double>(st, "Damage", 50.0);
        c.cost = Json::GetValue<double>(st, "Cost", 50.0);
        c.range = Json::GetValue<int>(st, "Range", 75);
        c.damage_type = JsonGet::GetDamageType(Json::GetValue<std::string_view>(st, "Damage Type"));
        c.surehit_type = JsonGet::GetSurehitType(Json::GetValue<std::string_view>(st, "Surehit Type"));
        c.refinement = JsonGet::GetRefinement(Json::GetValue<std::string_view>(st, "Refinement"));
    }
}

std::optional<Domain> JsonGet::GetDomain(std::string_view sv) {
    static const auto& m = UMaps::GetDMMap();
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    for (const auto& c : Json::LoadEntity<Domain>()){
        if (c.identity.name == sv) return c;
    }
    return std::nullopt;
}

SurehitType JsonGet::GetSurehitType(std::string_view sv) {
    static const auto& m = UMaps::GetSHMap();
    for (const auto& c : m){
        if (c.first == sv){
            return c.second;
        }
    }

    return SurehitType::Normal;
}
Refinement JsonGet::GetRefinement(std::string_view sv) {
    static const auto& m = UMaps::GetRFMap();
    for (const auto& c : m){
        if (c.first == sv){
            return c.second;
        }
    }
    return Refinement::Standard;
}