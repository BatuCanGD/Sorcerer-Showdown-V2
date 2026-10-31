#include "../../header/Json.hpp"

void JsonModify::ModifyTechnique(const nlohmann::json &j, Technique& c){
    if (j.contains("Identity")){
        const auto& id = j.at("Identity");
        c.identity.name = Json::GetValue<std::string>(id, "Name", "Delamain");
        c.identity.color = Json::GetValue<std::string>(id, "Color", "\x1b[38;5;117m");
        c.identity.description = Json::GetValue<std::string>(id, "Description");
    }
    c.barrier.can_use_barrier = Json::GetValue<bool>(j, "Barrier", false);
    c.barrier.is_active = c.barrier.can_use_barrier;
    if (j.contains("Abilities") && j.at("Abilities").is_array()){
        for (const auto& jr : j.at("Abilities")){
            TechAbility tb;
            if (jr.contains("Identity")){
                const auto& jri = jr.at("Identity");
                tb.id.name = Json::GetValue<std::string>(jri, "Name");
                tb.id.color = Json::GetValue<std::string>(jri, "Color");
                tb.id.description = Json::GetValue<std::string>(jri, "Description");
            }

            if (jr.contains("Stats")) {
                const auto& st = jr.at("Stats");
                tb.damage = Json::GetValue<double>(st, "Damage");
                tb.damage_type = JsonGet::GetDamageType(Json::GetValue<std::string>(st, "Damage Type", "Normal"));
                tb.output = Json::GetValue<double>(st, "Output");
                tb.cost = Json::GetValue<double>(st, "Cost");
                tb.given_effect = JsonGet::GetStatusEffect(Json::GetValue<std::string>(st, "Inflicted Effect"));
            }
            c.abilities.push_back(tb);
        }
    }
}

std::optional<Technique> JsonGet::GetTechnique(std::string_view sv) {
    static const auto& m = UMaps::GetTCMap();
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    for (const auto& c : Json::LoadEntity<Technique>()){
        if (c.identity.name == sv) return c;
    }
    return std::nullopt;
}