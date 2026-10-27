#include "../header/Creator.hpp"
#include "../header/Json.hpp"
#include "../header/Logger.hpp"
#include "../header/Character.hpp"
#include "../header/General/CursedTool.hpp"
#include "../header/Sorcery/Technique.hpp"

#include <algorithm>
#include <optional>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <memory>

std::vector<std::unique_ptr<Character>> Json::LoadCharacters() {
    static constexpr const char* fl = "characters.json";
    static constexpr const char* js = "Characters";
    Log::Line("Looking for {} in: {}", fl, std::filesystem::current_path().string());
    std::ifstream file(fl);

    if (!file.is_open()) {
        Log::Error("Could not find {}!", fl);
        return {};
    }
    nlohmann::json data;
    try {
        file >> data;
    }catch (const nlohmann::json::parse_error& e) {
        Log::Error("JSON Parse Error: {}", e.what());
        return {};
    }
    std::vector<std::unique_ptr<Character>> bf;
    if (data.contains(js) && data[js].is_array()) {
        for (const auto& charData : data[js]) {
            bf.push_back(Json::BuildCharacter(charData));
        }
    }
    return bf;
}
std::vector<Technique> Json::LoadTechniques() {
    static constexpr const char* fl = "techniques.json";
    static constexpr const char* js = "Techniques";
    Log::Line("Looking for {} in: {}", fl, std::filesystem::current_path().string());
    std::ifstream file(fl);

    if (!file.is_open()) {
        Log::Error("Could not find {}!", fl);
        return {};
    }
    nlohmann::json data;
    try {
        file >> data;
    }catch (const nlohmann::json::parse_error& e) {
        Log::Error("JSON Parse Error: {}", e.what());
        return {};
    }

    std::vector<Technique> bf;
    if (data.contains(js) && data[js].is_array()) {
        for (const auto& charData : data[js]) {
            bf.push_back(Json::BuildTechnique(charData));
        }
    }
    return bf;
}
std::vector<Domain> Json::LoadDomains() {
    static constexpr const char* fl = "domains.json";
    static constexpr const char* js = "Domains";
    Log::Line("Looking for {} in: {}", fl, std::filesystem::current_path().string());
    std::ifstream file(fl);

    if (!file.is_open()) {
        Log::Error("Could not find {}!", fl);
        return {};
    }

    nlohmann::json data;
    try {
        file >> data;
    }catch (const nlohmann::json::parse_error& e) {
        Log::Error("JSON Parse Error: {}", e.what());
        return {};
    }
    std::vector<Domain> bf;
    if (data.contains("Domains") && data["Domains"].is_array()) {
        for (const auto& charData : data[js]) {
            bf.push_back(Json::BuildDomain(charData));
        }
    }
    return bf;
}
std::vector<CursedTool> Json::LoadCursedTools() {
    static constexpr const char* fl = "cursedtools.json";
    static constexpr const char* js = "Cursed Tools";
    Log::Line("Looking for {} in: {}", fl, std::filesystem::current_path().string());
    std::ifstream file(fl);

    if (!file.is_open()) {
        Log::Error("Could not find {}!", fl);
        return {};
    }

    nlohmann::json data;
    try {
        file >> data;
    }catch (const nlohmann::json::parse_error& e) {
        Log::Error("JSON Parse Error: {}", e.what());
        return {};
    }
    std::vector<CursedTool> bf;
    if (data.contains(js) && data[js].is_array()) {
        for (const auto& charData : data[js]) {
            bf.push_back(Json::BuildTool(charData));
        }
    }
    return bf;
}
std::vector<Shikigami> Json::LoadShikigami() {
    static constexpr const char* fl = "shikigami.json";
    static constexpr const char* js = "Shikigami";
    Log::Line("Looking for {} in: {}", fl, std::filesystem::current_path().string());
    std::ifstream file(fl);

    if (!file.is_open()) {
        Log::Error("Could not find {}!", fl);
        return {};
    }
    nlohmann::json data;
    try {
        file >> data;
    }catch (const nlohmann::json::parse_error& e) {
        Log::Error("JSON Parse Error: {}", e.what());
        return {};
    }
    std::vector<Shikigami> bf;
    if (data.contains(js) && data[js].is_array()) {
        for (const auto& charData : data[js]) {
            bf.push_back(Json::BuildShikigami(charData));
        }
    }
    return bf;
}

std::vector<StatusEffect> Json::LoadEffects() {
    static constexpr const char* fl = "statuseffects.json";
    static constexpr const char* js = "Status Effects";
    Log::Line("Looking for {} in: {}", fl, std::filesystem::current_path().string());
    std::ifstream file(fl);

    if (!file.is_open()) {
        Log::Error("Could not find {}!", fl);
        return {};
    }
    nlohmann::json data;
    try {
        file >> data;
    }catch (const nlohmann::json::parse_error& e) {
        Log::Error("JSON Parse Error: {}", e.what());
        return {};
    }
    std::vector<StatusEffect> bf;
    if (data.contains(js) && data[js].is_array()) {
        for (const auto& charData : data[js]) {
            bf.push_back(Json::BuildEffect(charData));
        }
    }
    return bf;
}


std::unique_ptr<Character> Json::BuildCharacter(const nlohmann::json &j){
    std::unique_ptr<Character> c{std::make_unique<Character>()};

    if (j.contains("Type") && j.at("Type").is_string()){
        const auto& type = j.at("Type").get<std::string_view>();
        if (type == "Curse User"){
            c->curseUser = Sorcery::CurseUserSystem();
        }else{
            Log::Error("Invalid Type, defaulting to Character Type");
        }
    }else{
        Log::Error("\"Type\" doesnt exist, defaulting to Character Type");
    }

    Json::GetBaseValues(j, *c);
    if (j.contains("Sorcery")) {
        if (auto& cr = c->curseUser){
            Json::GetSorceryValues(j.at("Sorcery"), *cr);
        }
    }
    return c;
}

template <typename T>
T GetValue(const nlohmann::json& j, std::string_view sv, T default_val = T{}){
    if (j.contains(sv)){
        return j.at(sv).get<T>();
    }
    Log::Error("\"{}\" couldn't be found. default values were applied", sv);
    return default_val;
}

bool ContainsArray(const nlohmann::json& j, std::string_view sv){
    if (j.contains(sv) && j.at(sv).is_array()) {
        return true;
    }
    Log::Error("\"{}\" Array couldnt be found", sv);
    return false;
}

CursedTool Json::BuildTool(const nlohmann::json &j){
    CursedTool c{};

    if (j.contains("Identity")){
        const auto& id = j.at("Identity");
        c.identity.name = GetValue<std::string>(id, "Name", "Delamain");
        c.identity.color = GetValue<std::string>(id, "Color", "\x1b[38;5;117m");
        c.identity.description = GetValue<std::string>(id, "Description");
    }
    if (j.contains("Stats")) {
        const auto& st = j.at("Stats");
        c.damage = GetValue<double>(st, "Damage", 50.0);
        c.damage_type = Json::GetDamageType(GetValue<std::string_view>(st, "Damage Type"));
        c.given_effect = Json::GetStatusEffect(GetValue<std::string_view>(st, "Status Effect"));
    }

    return c;
}
Domain Json::BuildDomain(const nlohmann::json &j){
    Domain c{};

    return c;
}
Technique Json::BuildTechnique(const nlohmann::json &j){
    Technique c{};

    return c;
}
Shikigami Json::BuildShikigami(const nlohmann::json &j){
    Shikigami c{};

    return c;
}
StatusEffect Json::BuildEffect(const nlohmann::json &j) {
    StatusEffect c{};

    return c;
}

void Json::GetBaseValues(const nlohmann::json& j, Character& c) {
    if (j.contains("Identity")){
        const auto& id = j.at("Identity");
        c.identity.name = GetValue<std::string>(id, "Name", "Delamain");
        c.identity.color = GetValue<std::string>(id, "Color", "\x1b[38;5;117m");
        c.identity.description = GetValue<std::string>(id, "Description");
    }
    if (j.contains("Stats")){
        const auto& stats = j.at("Stats");
        c.state.max_health = GetValue<double>(stats, "Health", 50.0);
        c.state.health = c.state.max_health;
        c.state.strength = GetValue<double>(stats, "Strength", 25.0);
        c.state.durability = GetValue<double>(stats, "Durability", 15.0);
        c.state.is_invulnerable = GetValue<bool>(stats, "Undefeatable", false);
    }
    if (j.contains("Brain")){
        const auto& style = j.at("Brain");
        c.brain.fighting_style = Json::GetFightingStyle(GetValue<std::string_view>(style, "Fighting Style", "Mixed")); 
        c.brain.resource_usage = Json::GetResourceUsage(GetValue<std::string_view>(style, "Resource Usage", "Mixed"));
        c.brain.targeted_type  = Json::GetTargetedTypes(GetValue<std::string_view>(style, "Targeted Type", "Mixed"));
        c.brain.targeting_type = Json::GetTargetingType(GetValue<std::string_view>(style, "Targeting Type", "Random"));
    }
    if (j.contains("Equipment")){
        const auto& eq = j.at("Equipment");
        c.equipment.current_tool = GetCursedTool(GetValue<std::string_view>(eq, "Held Tool"));
        c.equipment.stored_tool = GetCursedTool(GetValue<std::string_view>(eq, "Stored Tool"));
        c.equipment.has_access_to_inventory = GetValue<bool>(eq, "Inventory Access");
        Json::ModifyInventory(eq, c.equipment.inventory);
    }
}

void Json::GetSorceryValues(const nlohmann::json& j, Sorcery::CurseUserSystem& c) {
    if (j.contains("Jujutsu")){
        const auto& jj = j.at("Jujutsu");
        c.jujutsu.technique = Json::GetTechnique(GetValue<std::string_view>(jj, "Technique"));
        c.jujutsu.domain = Json::GetDomain(GetValue<std::string_view>(jj, "Domain"));
        c.jujutsu.neutralizer = Json::GetNeutralizer(GetValue<std::string_view>(jj, "Neutralizer"));
        Json::ModifyShikigami(jj, c.jujutsu.shikigami);
    }
    if (j.contains("Cursed Energy")) {
        const auto& cs = j.at("Cursed Energy");
        c.ce_system.max_cursed_energy = GetValue<double>(cs, "Amount", 500.0);
        c.ce_system.cursed_energy = c.ce_system.max_cursed_energy;
        c.ce_system.regeneration_amount = GetValue<double>(cs, "Regen Amount", 25.0);
        c.ce_system.bf_chance = static_cast<std::uint8_t>(std::clamp(GetValue<unsigned int>(cs, "Blackflash Chance", 5u), 0u, 100u));
        c.ce_system.efficiency = Json::GetEfficiency(GetValue<std::string_view>(cs, "Efficiency"));
    }
    if (j.contains("Cursed Energy Output")){
        c.output.max_output_potential = GetValue<double>(j.at("Cursed Energy Output"), "Maximum", 100.0);
    }
    c.amplification.can_use_amp = GetValue<bool>(j, "Domain Amplification");
    c.rct_system.can_use_rct = GetValue<bool>(j, "Reverse Cursed Technique");
    if (j.contains("Traits")){
        const auto& tr = j.at("Traits");
        c.traits.passive_healing = GetValue<bool>(tr, "Passive Healing", false);
        c.traits.six_eyes = GetValue<bool>(tr, "Six Eyes", false);
    }
}

Sorcery::CursedEnergySystem::Efficiency Json::GetEfficiency(std::string_view sv){
    static const UMap<Sorcery::CursedEnergySystem::Efficiency> m {
        {"Peak", Sorcery::CursedEnergySystem::Efficiency::Peak},
        {"Extreme", Sorcery::CursedEnergySystem::Efficiency::Extreme},
        {"Expert", Sorcery::CursedEnergySystem::Efficiency::Expert},
        {"Precise", Sorcery::CursedEnergySystem::Efficiency::Precise},
        {"Stable", Sorcery::CursedEnergySystem::Efficiency::Stable},
        {"Unstable", Sorcery::CursedEnergySystem::Efficiency::Unstable},
        {"Rough", Sorcery::CursedEnergySystem::Efficiency::Rough},
        {"Wasteful", Sorcery::CursedEnergySystem::Efficiency::Wasteful}
    };
    for(const auto& c : m){
        if (sv == c.first){
            return c.second;
        }
    }
    return Sorcery::CursedEnergySystem::Efficiency::Stable;
}
DamageType Json::GetDamageType(std::string_view sv){
    static const UMap<DamageType> m {
        {"Bypass All", DamageType::BypassAll},
        {"Bypass Reinforcement", DamageType::BypassRein},
        {"Bypass Techniques", DamageType::BypassTech},
    };
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    return DamageType::Normal;
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

UMap<CursedTool> UMaps::GetCTMap() {
    return {
        {"Split Soul Katana", Create::Tool::SplitSoulKatana()},
        {"Inverted Spear of Heaven", Create::Tool::InvertedSpearOfHeaven()},
        {"Playful Cloud", Create::Tool::PlayfulCloud()}
    };
}

UMap<Shikigami> UMaps::GetSKMap() {
    return {
        {"Mahoraga", Create::Shk::Mahoraga()},
        {"Agito", Create::Shk::Agito()},
        {"Ember Insects", Create::Shk::EmberInsects()}
    };
}

UMap<Technique> UMaps::GetTCMap() {
    return {
        {"Limitless", Create::Tech::Limitless()},
        {"Shrine", Create::Tech::Shrine()},
        {"Idle Transfiguration", Create::Tech::IdleTransfiguration()}
    };
}

UMap<Domain> UMaps::GetDMMap() {
    return {
        {"Unlimited Void", Create::Dmn::UnlimitedVoid()},
        {"Malevolent Shrine", Create::Dmn::MalevolentShrine()},
        {"Self Embodiment Of Perfection", Create::Dmn::SelfEmbodimentOfPerfection()}
    };
}

UMap<Neutralizer> UMaps::GetNLMap() {
    return {
        {"Hollow Wicker Basket", Create::Nlr::HollowWickerBasket()},
        {"Simple Domain", Create::Nlr::SimpleDomain()},
        {"Falling Blossom Emotion", Create::Nlr::FallingBlossomEmotion()}
    };
}

UMap<StatusEffect> UMaps::GetSTMap() {
    return {
        {"Bleed", Create::Effect::Bleed()}
    };
}

void Json::ModifyInventory(const nlohmann::json& j, std::vector<CursedTool>& t) {
    const auto& m = UMaps::GetCTMap();
    if (ContainsArray(j, "Inventory")){
        for (const auto& c : j.at("Inventory").get<std::vector<std::string>>()){
            const auto b = m.find(c);
            if (b != m.end()){
                t.push_back(b->second);
            }else{
                for(const auto& tl : Json::LoadCursedTools()){
                    if (tl.identity.name == c){
                        t.push_back(tl);
                    }
                }
            }
        }
    }
}

void Json::ModifyShikigami(const nlohmann::json& j, std::vector<Shikigami>& t){
    const auto& m = UMaps::GetSKMap();
    if (ContainsArray(j, "Shikigami")){
        for (const auto& c : j.at("Shikigami").get<std::vector<std::string>>()){
            const auto b = m.find(c);
            if (b != m.end()){
                t.push_back(b->second);
            }else{
                for (const auto& sc : Json::LoadShikigami()){
                    if (sc.id.name == c){
                        t.push_back(sc);
                    }
                }
            }

        }
    }
}

std::optional<CursedTool> Json::GetCursedTool(std::string_view sv){
    const auto& m = UMaps::GetCTMap();
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    for (const auto& c : Json::LoadCursedTools()){
        if (c.identity.name == sv) return c;
    }
    return std::nullopt;
}

std::optional<StatusEffect> Json::GetStatusEffect(std::string_view sv){
    const auto& m = UMaps::GetSTMap();
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    for (const auto& c : Json::LoadEffects()){
        if (c.id.name == sv) return c;
    }
    return std::nullopt;
}

std::optional<Technique> Json::GetTechnique(std::string_view sv) {
    const auto& m = UMaps::GetTCMap();
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    for (const auto& c : Json::LoadTechniques()){
        if (c.identity.name == sv) return c;
    }
    return std::nullopt;
}
std::optional<Domain> Json::GetDomain(std::string_view sv) {
    const auto& m = UMaps::GetDMMap();
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    for (const auto& c : Json::LoadDomains()){
        if (c.identity.name == sv) return c;
    }
    return std::nullopt;
}
std::optional<Neutralizer> Json::GetNeutralizer(std::string_view sv) {
    const auto& m = UMaps::GetNLMap();
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    return std::nullopt;
}
