#include "../header/Creator.hpp"
#include "../header/Json.hpp"
#include "../header/CharacterType/Character.hpp"
#include "../header/CharacterType/CurseUser.hpp"
#include "../header/General/CursedTool.hpp"
#include "../header/Sorcery/Technique.hpp"
#include "../header/Enums.hpp"
#include <unordered_map>
#include <memory>



std::unique_ptr<Character> Create::Char::Custom(const nlohmann::json &j){
    std::unique_ptr<Character> c{};

    if (j.contains("Type")){
        if (j.at("Type").contains("Curse User")){
            c = std::make_unique<CurseUser>();
        }else{
            c = std::make_unique<Character>();
        }
    }else{
        c = std::make_unique<Character>();
    }
    Json::GetDefaultValues(j, *c);
    if (auto* cr = c->CanUseSorcery()){
        Json::GetCurseUserValues(j, *cr);
    }
    return c;
}

template <typename T>
T GetValueAt(const nlohmann::json& j, std::string_view sv, T default_val = T{}){
    if (j.contains(sv)){
        return j.at(sv).get<T>();
    }
    return default_val;
}
bool ContainsArray(const nlohmann::json& j, std::string_view sv){
    return j.contains(sv) || (j.contains(sv) && j.at(sv).is_array());
}

void Json::GetDefaultValues(const nlohmann::json& j, Character& c) {
    if (ContainsArray(j, "Identity")){
        const auto& id = j.at("Identity");
        c.Identity().name = GetValueAt<std::string>(id, "Name", "Delamain");
        c.Identity().color = GetValueAt<std::string>(id, "Color", "\x1b[38;5;117m");
        c.Identity().description = GetValueAt<std::string>(id, "Description");
    }
    if (ContainsArray(j, "Stats")){
        const auto& stats = j.at("Stats");
        c.State().max_health = GetValueAt<double>(stats, "Health", 50.0);
        c.State().health = c.State().max_health;
        c.State().strength = GetValueAt<double>(stats, "Strength", 25.0);
        c.State().durability = GetValueAt<double>(stats, "Durability", 15.0);
        c.State().is_invulnerable = GetValueAt<bool>(stats, "Undefeatable", false);
    }
    if (ContainsArray(j, "Brain")){
        const auto& style = j.at("Brain");
        c.Style().fighting_style = Json::GetFightingStyle(GetValueAt<std::string_view>(style, "Fighting Style", "Mixed")); 
        c.Style().resource_usage = Json::GetResourceUsage(GetValueAt<std::string_view>(style, "Resource Usage", "Mixed"));
        c.Style().targeted_type  = Json::GetTargetedTypes(GetValueAt<std::string_view>(style, "Targeted Type", "Mixed"));
        c.Style().targeting_type = Json::GetTargetingType(GetValueAt<std::string_view>(style, "Targeting Type", "Random"));
    }
    if (ContainsArray(j, "Equipment")){
        const auto& eq = j.at("Equipment");
        c.Equipment().has_access_to_inventory = GetValueAt<bool>(eq, "Inventory Access");
    }
}
void Json::GetCurseUserValues(const nlohmann::json& j, CurseUser& c) {

}

template <typename T>
using UMap = std::unordered_map<std::string, T>;

std::optional<Shikigami> Json::GetShikigami(std::string_view sv) {
    static const UMap<Shikigami> m {
        {"Mahoraga", Create::Shk::Mahoraga()},
        {"Agito", Create::Shk::Agito()},
        {"Ember Insects", Create::Shk::EmberInsects()}
    };
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    return std::nullopt;
}
std::optional<Technique> Json::GetTechnique(std::string_view sv) {
    static const UMap<Technique> m {
        {"Limitless", Create::Tech::Limitless()},
        {"Shrine", Create::Tech::Shrine()},
        {"Idle Transfiguration", Create::Tech::IdleTransfiguration()}
    };
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }

    return std::nullopt;
}
std::optional<Domain> Json::GetDomain(std::string_view sv) {
    static const UMap<Domain> m {
        {"Unlimited Void", Create::Dmn::UnlimitedVoid()},
        {"Malevolent Shrine", Create::Dmn::MalevolentShrine()},
        {"Self Embodiment Of Perfection", Create::Dmn::SelfEmbodimentOfPerfection()}
    };
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    return std::nullopt;
}
std::optional<Neutralizer> Json::GetNeutralizer(std::string_view sv) {
    static const UMap<Neutralizer> m {
        {"Mahoraga", Create::Nlr::HollowWickerBasket()},
        {"Agito", Create::Nlr::SimpleDomain()},
        {"Ember Insects", Create::Nlr::FallingBlossomEmotion()}
    };
    for (const auto& c : m) {
        if (c.first == sv){
            return c.second;
        }
    }
    return std::nullopt;
}
