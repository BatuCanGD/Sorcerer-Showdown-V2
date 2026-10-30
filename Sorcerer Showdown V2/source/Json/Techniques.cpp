#include "../../header/Json.hpp"

void JsonModify::ModifyTechnique(const nlohmann::json &j, Technique& c){
    if (j.contains("Identity")){
        const auto& id = j.at("Identity");
        c.identity.name = Json::GetValue<std::string>(id, "Name", "Delamain");
        c.identity.color = Json::GetValue<std::string>(id, "Color", "\x1b[38;5;117m");
        c.identity.description = Json::GetValue<std::string>(id, "Description");
    }
    if (j.contains("Abilities") && j.at("Abilities").is_array()){
        for (const auto& jr : j.at("Abilities")){
            TechAbility tb;
            tb.id.name = Json::GetValue<std::string>(jr, "Name");
            tb.id.color = Json::GetValue<std::string>(jr, "Color");
            tb.id.description = Json::GetValue<std::string>(jr, "Description");
            tb.damage = Json::GetValue<double>(jr, "Damage");
            tb.damage_type = JsonGet::GetDamageType(Json::GetValue<std::string>(jr, "Damage Type", "Normal"));
            tb.output = Json::GetValue<double>(jr, "Output");
            tb.cost = Json::GetValue<double>(jr, "Cost");
            tb.given_effect = JsonGet::GetStatusEffect(Json::GetValue<std::string>(jr, "Inflicted Effect"));
            c.abilities.push_back(tb);
        }
    }
}