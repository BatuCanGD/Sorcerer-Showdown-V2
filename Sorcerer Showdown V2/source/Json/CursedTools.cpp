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