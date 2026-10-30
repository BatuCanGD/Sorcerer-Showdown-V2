#include "../../header/Json.hpp"
#include "../../header/General/CursedTool.hpp"

void JsonModify::ModifyCursedTool(const nlohmann::json &j, CursedTool& c){
    if (j.contains("Identity")){
        const auto& id = j.at("Identity");
        c.identity.name = Json::GetValue<std::string>(id, "Name", "Delamain");
        c.identity.color = Json::GetValue<std::string>(id, "Color", "\x1b[38;5;117m");
        c.identity.description = Json::GetValue<std::string>(id, "Description");
    }
    if (j.contains("Stats")) {
        const auto& st = j.at("Stats");
        c.damage = Json::GetValue<double>(st, "Damage", 50.0);
        c.damage_type = JsonGet::GetDamageType(Json::GetValue<std::string_view>(st, "Damage Type"));
        c.given_effect = JsonGet::GetStatusEffect(Json::GetValue<std::string_view>(st, "Inflicted Effect"));
    }
}

std::vector<CursedTool> JsonGet::GetInventory(const nlohmann::json& j) {
    static const auto& m = UMaps::GetCTMap();
    std::vector<CursedTool> t;
    if (Json::ContainsArray(j, "Inventory")){
        for (const auto& c : j.at("Inventory").get<std::vector<std::string>>()){
            const auto b = m.find(c);
            if (b != m.end()){
                t.push_back(b->second);
            }else{
                for(const auto& tl : Json::LoadEntity<CursedTool>()){
                    if (tl.identity.name == c){
                        t.push_back(tl);
                    }
                }
            }
        }
    }
    return t;
}

std::optional<CursedTool> JsonGet::GetCursedTool(std::string_view sv){
    static const auto& m = UMaps::GetCTMap();
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    for (const auto& c : Json::LoadEntity<CursedTool>()){
        if (c.identity.name == sv) return c;
    }
    return std::nullopt;
}

