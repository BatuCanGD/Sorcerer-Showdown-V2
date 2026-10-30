#include "../../header/Json.hpp"

void JsonModify::ModifyStatusEffect(const nlohmann::json &j, StatusEffect& c) {
    if (j.contains("Identity")){
        const auto& id = j.at("Identity");
        c.id.name = Json::GetValue<std::string>(id, "Name", "Delamain");
        c.id.color = Json::GetValue<std::string>(id, "Color", "\x1b[38;5;117m");
        c.id.description = Json::GetValue<std::string>(id, "Description");
    }
    if (j.contains("Stats")) {
        const auto& st = j.at("Stats");
        c.effect_type = Json::GetValue<std::string_view>(st, "Effect Type") == "Regenerating" ? EffectType::Increase : EffectType::Decrease;
        c.effect_for_type = JsonGet::GetEffectForType(Json::GetValue<std::string_view>(st, "Affected Stat"));
        c.effect_amount = Json::GetValue<double>(st, "Effect Strength");
        c.turn_amount = Json::GetValue<int>(st, "Turn Amount", 3);
    }
}

EffectForType JsonGet::GetEffectForType(std::string_view sv) {
    static const UMap<EffectForType> m {
        {"Health", EffectForType::Health},
        {"Durability", EffectForType::Durability},
        {"Strength", EffectForType::Strength},
        {"Cursed Energy", EffectForType::CursedEnergy},
        {"Max Output", EffectForType::MaxOutput}
    };
    for (const auto& c : m){
        if (c.first == sv){
            return c.second;
        }
    }
    return EffectForType::None;
}
