#include "../header/Creator.hpp"
#include "../header/Json.hpp"
#include "../header/Logger.hpp"
#include "../header/CharacterType/Character.hpp"
#include "../header/CharacterType/CurseUser.hpp"
#include "../header/General/CursedTool.hpp"
#include "../header/Sorcery/Technique.hpp"

#include <algorithm>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <memory>

void Json::LoadCharacters(std::vector<std::unique_ptr<Character>>& bf) {
    Log::Line("Looking for characters.json in: {}", std::filesystem::current_path().string());
    std::ifstream file("Characters.json");

    if (!file.is_open()) {
        Log::Error("Could not find characters.json!");
        return;
    }
    nlohmann::json data;
    try {
        file >> data;
    }catch (const nlohmann::json::parse_error& e) {
        Log::Error("JSON Parse Error: {}", e.what());
        return;
    }

    if (data.contains("Characters") && data["Characters"].is_array()) {
        for (const auto& charData : data["Characters"]) {
            bf.push_back(std::move(Json::BuildCharacter(charData)));
        }
    }
}


std::unique_ptr<Character> Json::BuildCharacter(const nlohmann::json &j){
    std::unique_ptr<Character> c{};

    if (j.contains("Type") && j.at("Type").is_string()){
        const auto& type = j.at("Type").get<std::string_view>();
        if (type == "Curse User"){
            c = std::make_unique<CurseUser>();
        }
    }
    if (!c) {
        c = std::make_unique<Character>();
    }

    Json::GetDefaultValues(j, *c);
    if (j.contains("Sorcery")) {
        if (auto* cr = c->CanUseSorcery()){
            Json::GetCurseUserValues(j.at("Sorcery"), *cr);
        }
    }
    return c;
}

template <typename T>
T GetValue(const nlohmann::json& j, std::string_view sv, T default_val = T{}){
    if (j.contains(sv)){
        return j.at(sv).get<T>();
    }
    return default_val;
}

bool ContainsArray(const nlohmann::json& j, std::string_view sv){
    return (j.contains(sv) && j.at(sv).is_array());
}

void Json::GetDefaultValues(const nlohmann::json& j, Character& c) {
    if (j.contains("Identity")){
        const auto& id = j.at("Identity");
        c.Identity().name = GetValue<std::string>(id, "Name", "Delamain");
        c.Identity().color = GetValue<std::string>(id, "Color", "\x1b[38;5;117m");
        c.Identity().description = GetValue<std::string>(id, "Description");
    }
    if (j.contains("Stats")){
        const auto& stats = j.at("Stats");
        c.State().max_health = GetValue<double>(stats, "Health", 50.0);
        c.State().health = c.State().max_health;
        c.State().strength = GetValue<double>(stats, "Strength", 25.0);
        c.State().durability = GetValue<double>(stats, "Durability", 15.0);
        c.State().is_invulnerable = GetValue<bool>(stats, "Undefeatable", false);
    }
    if (j.contains("Brain")){
        const auto& style = j.at("Brain");
        c.Style().fighting_style = Json::GetFightingStyle(GetValue<std::string_view>(style, "Fighting Style", "Mixed")); 
        c.Style().resource_usage = Json::GetResourceUsage(GetValue<std::string_view>(style, "Resource Usage", "Mixed"));
        c.Style().targeted_type  = Json::GetTargetedTypes(GetValue<std::string_view>(style, "Targeted Type", "Mixed"));
        c.Style().targeting_type = Json::GetTargetingType(GetValue<std::string_view>(style, "Targeting Type", "Random"));
    }
    if (j.contains("Equipment")){
        const auto& eq = j.at("Equipment");
        c.Equipment().current_tool = GetCursedTool(GetValue<std::string_view>(eq, "Held Tool"));
        c.Equipment().stored_tool = GetCursedTool(GetValue<std::string_view>(eq, "Stored Tool"));
        c.Equipment().has_access_to_inventory = GetValue<bool>(eq, "Inventory Access");
        Json::ModifyInventory(eq, c.Equipment().inventory);
    }
}

void Json::GetCurseUserValues(const nlohmann::json& j, CurseUser& c) {
    if (j.contains("Jujutsu")){
        const auto& jj = j.at("Jujutsu");
        c.Jujutsu().technique = Json::GetTechnique(GetValue<std::string_view>(jj, "Technique"));
        c.Jujutsu().domain = Json::GetDomain(GetValue<std::string_view>(jj, "Domain"));
        c.Jujutsu().neutralizer = Json::GetNeutralizer(GetValue<std::string_view>(jj, "Neutralizer"));
        Json::ModifyShikigami(jj, c.Jujutsu().shikigami);
    }
    if (j.contains("Cursed Energy")) {
        const auto& cs = j.at("Cursed Energy");
        c.CursedEnergy().max_cursed_energy = GetValue<double>(cs, "Amount", 500.0);
        c.CursedEnergy().cursed_energy = c.CursedEnergy().max_cursed_energy;
        c.CursedEnergy().regeneration_amount = GetValue<double>(cs, "Regen Amount", 25.0);
        c.CursedEnergy().bf_chance = static_cast<std::uint8_t>(std::clamp(GetValue<unsigned int>(cs, "Blackflash Chance", 5u), 0u, 100u));
        c.CursedEnergy().efficiency = Json::GetEfficiency(GetValue<std::string_view>(cs, "Efficiency"));
    }
    if (j.contains("Cursed Energy Output")){
        c.Output().max_output_potential = GetValue<double>(j.at("Cursed Energy Output"), "Maximum", 100.0);
    }
    c.Amplification().can_use_amp = GetValue<bool>(j, "Domain Amplification");
    c.RCT().can_use_rct = GetValue<bool>(j, "Reverse Cursed Technique");
    if (j.contains("Traits")){
        const auto& tr = j.at("Traits");
        c.Traits().passive_healing = GetValue<bool>(tr, "Passive Healing", false);
        c.Traits().six_eyes = GetValue<bool>(tr, "Six Eyes", false);
    }
}

template <typename T>
using UMap = std::unordered_map<std::string, T>;

CursedEnergySystem::Efficiency Json::GetEfficiency(std::string_view sv){
    static const UMap<CursedEnergySystem::Efficiency> m {
        {"Peak", CursedEnergySystem::Efficiency::Peak},
        {"Extreme", CursedEnergySystem::Efficiency::Extreme},
        {"Expert", CursedEnergySystem::Efficiency::Expert},
        {"Precise", CursedEnergySystem::Efficiency::Precise},
        {"Stable", CursedEnergySystem::Efficiency::Stable},
        {"Unstable", CursedEnergySystem::Efficiency::Unstable},
        {"Rough", CursedEnergySystem::Efficiency::Rough},
        {"Wasteful", CursedEnergySystem::Efficiency::Wasteful}
    };
    for(const auto& c : m){
        if (sv == c.first){
            return c.second;
        }
    }
    return CursedEnergySystem::Efficiency::Stable;
}


TargetedTypes Json::GetTargetedTypes(std::string_view sv) {
    return sv == "Mixed" ? TargetedTypes::Mixed : TargetedTypes::Character;
}

TargetingType Json::GetTargetingType(std::string_view sv) {
    static const UMap<TargetingType> m {
        {"Highest HP", TargetingType::HighestHP},
        {"Lowest HP", TargetingType::LowestHP}
    };
    for (const auto& s : m){
        if (s.first == sv){
            return s.second;
        }
    }
    return TargetingType::Random;
}

ResourceUsage Json::GetResourceUsage(std::string_view sv){
    static const UMap<ResourceUsage> m {
        {"All Out", ResourceUsage::AllOut},
        {"Conservative", ResourceUsage::Conservative}
    };
    for (const auto& s : m){
        if (s.first == sv){
            return s.second;
        }
    }
    return ResourceUsage::Mixed;
}


FightingStyle Json::GetFightingStyle(std::string_view sv) {
    static const UMap<FightingStyle> m {
        {"Aggressive", FightingStyle::Aggressive},
        {"Defensive", FightingStyle::Defensive},
    };
    for (const auto& s : m){
        if (s.first == sv){
            return s.second;
        }
    }
    return FightingStyle::Mixed;
}

UMap<CursedTool> GetCTMap() {
    return {
        {"Split Soul Katana", Create::Tool::SplitSoulKatana()},
        {"Inverted Spear of Heaven", Create::Tool::InvertedSpearOfHeaven()},
        {"Playful Cloud", Create::Tool::PlayfulCloud()}
    };
}

UMap<Shikigami> GetSKMap() {
    return {
        {"Mahoraga", Create::Shk::Mahoraga()},
        {"Agito", Create::Shk::Agito()},
        {"Ember Insects", Create::Shk::EmberInsects()}
    };
}

UMap<Technique> GetTCMap() {
    return {
        {"Limitless", Create::Tech::Limitless()},
        {"Shrine", Create::Tech::Shrine()},
        {"Idle Transfiguration", Create::Tech::IdleTransfiguration()}
    };
}

UMap<Domain> GetDMMap() {
    return {
        {"Unlimited Void", Create::Dmn::UnlimitedVoid()},
        {"Malevolent Shrine", Create::Dmn::MalevolentShrine()},
        {"Self Embodiment Of Perfection", Create::Dmn::SelfEmbodimentOfPerfection()}
    };
}

UMap<Neutralizer> GetNLMap() {
    return {
        {"Hollow Wicker Basket", Create::Nlr::HollowWickerBasket()},
        {"Simple Domain", Create::Nlr::SimpleDomain()},
        {"Falling Blossom Emotion", Create::Nlr::FallingBlossomEmotion()}
    };
}


void Json::ModifyInventory(const nlohmann::json& j, std::vector<CursedTool>& t) {
    const auto& m = GetCTMap();
    if (ContainsArray(j, "Inventory")){
        for (const auto& c : j.at("Inventory").get<std::vector<std::string>>()){
            const auto b = m.find(c);
            if (b != m.end()){
                t.push_back(b->second);
            }
        }
    }
}

void Json::ModifyShikigami(const nlohmann::json& j, std::vector<Shikigami>& t){
    const auto& m = GetSKMap();
    if (ContainsArray(j, "Shikigami")){
        for (const auto& c : j.at("Shikigami").get<std::vector<std::string>>()){
            const auto b = m.find(c);
            if (b != m.end()){
                t.push_back(b->second);
            }
        }
    }
}

std::optional<CursedTool> Json::GetCursedTool(std::string_view sv){
    const auto& m = GetCTMap();
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    return std::nullopt;
}

std::optional<Technique> Json::GetTechnique(std::string_view sv) {
    const auto& m = GetTCMap();
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }

    return std::nullopt;
}
std::optional<Domain> Json::GetDomain(std::string_view sv) {
    const auto& m = GetDMMap();
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    return std::nullopt;
}
std::optional<Neutralizer> Json::GetNeutralizer(std::string_view sv) {
    const auto& m = GetNLMap();
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    return std::nullopt;
}
