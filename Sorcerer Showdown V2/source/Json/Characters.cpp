#include "../../header/Json.hpp"
#include "../../header/Creator.hpp"
#include "../../header/Character.hpp"

void JsonModify::ModifyCharacter(const nlohmann::json& j, Character& c) {
    ModifyType(j,c);
    ModifyBaseValues(j, c);
    ModifySorceryValues(j, c);
}

void JsonModify::ModifyType(const nlohmann::json &j, Character &c){
    if (j.contains("Type") && j.at("Type").is_string()){
        const auto& type = j.at("Type").get<std::string_view>();
        if (type == "Curse User"){
            c.curseUser = Sorcery::CurseUserSystem();
            Log::Line("Loaded Curse User type");
        }else if (type == "Character"){
            Log::Line("Loaded Character type");
        }else{
            Log::Error("Invalid Type, defaulting to Base Character type");
        }
    }else{
        Log::Error("\"Type\" doesnt exist, defaulting to Base Character type");
    }
}

void JsonModify::ModifyBaseValues(const nlohmann::json& j, Character& c) {
    if (j.contains("Identity")){
        const auto& id = j.at("Identity");
        c.identity.name = Json::GetValue<std::string>(id, "Name", "Delamain");
        c.identity.color = Json::GetValue<std::string>(id, "Color", "\x1b[38;5;117m");
        c.identity.description = Json::GetValue<std::string>(id, "Description");
    }
    if (j.contains("Stats")){
        const auto& stats = j.at("Stats");
        c.state.max_health = Json::GetValue<double>(stats, "Health", 50.0);
        c.state.health = c.state.max_health;
        c.state.strength = Json::GetValue<double>(stats, "Strength", 25.0);
        c.state.durability = Json::GetValue<double>(stats, "Durability", 15.0);
        c.state.is_invulnerable = Json::GetValue<bool>(stats, "Undefeatable", false);
    }
    if (j.contains("Brain")){
        const auto& style = j.at("Brain");
        c.brain.fighting_style = JsonGet::GetFightingStyle(Json::GetValue<std::string_view>(style, "Fighting Style", "Mixed")); 
        c.brain.resource_usage = JsonGet::GetResourceUsage(Json::GetValue<std::string_view>(style, "Resource Usage", "Mixed"));
        c.brain.targeted_type  = JsonGet::GetTargetedTypes(Json::GetValue<std::string_view>(style, "Targeted Type", "Mixed"));
        c.brain.targeting_type = JsonGet::GetTargetingType(Json::GetValue<std::string_view>(style, "Targeting Type", "Random"));
    }
    if (j.contains("Equipment")){
        const auto& eq = j.at("Equipment");
        c.equipment.current_tool = JsonGet::GetCursedTool(Json::GetValue<std::string_view>(eq, "Held Tool"));
        c.equipment.stored_tool = JsonGet::GetCursedTool(Json::GetValue<std::string_view>(eq, "Stored Tool"));
        c.equipment.has_access_to_inventory = Json::GetValue<bool>(eq, "Inventory Access");
        c.equipment.inventory = JsonGet::GetInventory(eq);
    }
}

void JsonModify::ModifySorceryValues(const nlohmann::json& son, Character& crs) {
    if (auto& c = crs.curseUser) {
        if (son.contains("Sorcery")) {
            const auto& j = son.at("Sorcery");
            if (j.contains("Jujutsu")){
                const auto& jj = j.at("Jujutsu");
                c->jujutsu.technique = JsonGet::GetTechnique(Json::GetValue<std::string_view>(jj, "Technique"));
                c->jujutsu.domain = JsonGet::GetDomain(Json::GetValue<std::string_view>(jj, "Domain"));
                c->jujutsu.neutralizer = JsonGet::GetNeutralizer(Json::GetValue<std::string_view>(jj, "Neutralizer"));
                c->jujutsu.shikigami = JsonGet::GetShikigami(jj);
            }
            if (j.contains("Cursed Energy")) {
                const auto& cs = j.at("Cursed Energy");
                c->ce_system.max_cursed_energy = Json::GetValue<double>(cs, "Amount", 500.0);
                c->ce_system.cursed_energy = c->ce_system.max_cursed_energy;
                c->ce_system.regeneration_amount = Json::GetValue<double>(cs, "Regen Amount", 25.0);
                c->ce_system.bf_chance = static_cast<std::uint8_t>(std::clamp(Json::GetValue<unsigned int>(cs, "Blackflash Chance", 5u), 0u, 100u));
                c->ce_system.efficiency = JsonGet::GetEfficiency(Json::GetValue<std::string_view>(cs, "Efficiency"));
            }
            if (j.contains("Cursed Energy Output")){
                c->output.max_output_potential = Json::GetValue<double>(j.at("Cursed Energy Output"), "Maximum", 100.0);
            }
            c->amplification.can_use_amp = Json::GetValue<bool>(j, "Domain Amplification");
            c->rct_system.can_use_rct = Json::GetValue<bool>(j, "Reverse Cursed Technique");
            if (j.contains("Traits")){
                const auto& tr = j.at("Traits");
                c->traits.passive_healing = Json::GetValue<bool>(tr, "Passive Healing", false);
                c->traits.six_eyes = Json::GetValue<bool>(tr, "Six Eyes", false);
            }  
        }
    }
}

Sorcery::CursedEnergySystem::Efficiency JsonGet::GetEfficiency(std::string_view sv){
    static const auto& m = UMaps::GetEFMap();
    for(const auto& c : m){
        if (sv == c.first){
            return c.second;
        }
    }
    return Sorcery::CursedEnergySystem::Efficiency::Stable;
}


TargetedTypes JsonGet::GetTargetedTypes(std::string_view sv) {
    return sv == "Mixed" ? TargetedTypes::Mixed : TargetedTypes::Character;
}

TargetingType JsonGet::GetTargetingType(std::string_view sv) {
    static const auto& m = UMaps::GetTTMap();
    for (const auto& s : m){
        if (s.first == sv){
            return s.second;
        }
    }
    return TargetingType::Random;
}

ResourceUsage JsonGet::GetResourceUsage(std::string_view sv){
    static const auto& m = UMaps::GetRUMap();
    for (const auto& s : m){
        if (s.first == sv){
            return s.second;
        }
    }
    return ResourceUsage::Mixed;
}


FightingStyle JsonGet::GetFightingStyle(std::string_view sv) {
    static const auto& m = UMaps::GetFSMap();
    for (const auto& s : m){
        if (s.first == sv){
            return s.second;
        }
    }
    return FightingStyle::Mixed;
}

