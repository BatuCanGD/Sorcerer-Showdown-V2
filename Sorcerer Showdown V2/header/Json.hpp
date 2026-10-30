#pragma once
#include "AI.hpp"
#include "General/StatusEffects.hpp"
#include "Sorcery/Domain.hpp"
#include "Sorcery/Shikigami.hpp"
#include "Logger.hpp"
#include "SorceryStruct.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <string_view>
#include <unordered_map>
#include <optional>

struct Character;
struct Shikigami;
struct CursedTool;
struct StatusEffect;
struct Technique;
struct Domain;
struct Neutralizer;

namespace JsonModify{
    void ModifyType(const nlohmann::json& j, Character& c);
    void ModifyBaseValues(const nlohmann::json& j, Character& c);
    void ModifySorceryValues(const nlohmann::json& j, Character& c);
    void ModifyCharacter(const nlohmann::json& j, Character& c);
    
    void ModifyCursedTool(const nlohmann::json& j, CursedTool& t);
    void ModifyTechnique(const nlohmann::json& j, Technique& t);
    void ModifyStatusEffect(const nlohmann::json& j, StatusEffect& t);
    void ModifyDomain(const nlohmann::json& j, Domain& t);
    void ModifyNeutralizer(const nlohmann::json& j, Neutralizer& t);
    void ModifyShikigami(const nlohmann::json& j, Shikigami& t);
    void ModifyInventory(const nlohmann::json& j, std::vector<CursedTool>& t);
    void ModifyShikigami(const nlohmann::json& j, std::vector<Shikigami>& t);
}

namespace JsonGet {
    FightingStyle GetFightingStyle(std::string_view sv);
    ResourceUsage GetResourceUsage(std::string_view sv);
    TargetingType GetTargetingType(std::string_view sv);
    TargetedTypes GetTargetedTypes(std::string_view sv);
    DamageType GetDamageType(std::string_view sv);
    SurehitType GetSurehitType(std::string_view sv);
    Refinement GetRefinement(std::string_view sv);
    ActiveAbility GetActiveAbility(std::string_view sv);
    SupportType GetSupportType(std::string_view sv);
    EffectForType GetEffectForType(std::string_view sv);
    Sorcery::CursedEnergySystem::Efficiency GetEfficiency(std::string_view sv);

    std::optional<CursedTool> GetCursedTool(std::string_view sv);
    std::optional<Neutralizer> GetNeutralizer(std::string_view sv);
    std::optional<Technique> GetTechnique(std::string_view sv);
    std::optional<StatusEffect> GetStatusEffect(std::string_view sv);
    std::optional<Domain> GetDomain(std::string_view sv);
}

namespace Json {
    template <typename T>
    inline T GetValue(const nlohmann::json& j, std::string_view sv, T default_val = T{}){
        if (j.contains(sv)){
            try {
                return j.at(sv).get<T>();
            }catch(const nlohmann::json::exception&) {
                Log::Error("\"{}\" has an invalid value type. default values were applied", sv);
                return default_val;
            }
        }
        Log::Error("\"{}\" couldn't be found. default values were applied", sv);
        return default_val;
    }

    inline bool ContainsArray(const nlohmann::json& j, std::string_view sv){
        if (j.contains(sv) && j.at(sv).is_array()) {
            return true;
        }
        Log::Error("\"{}\" Array couldnt be found", sv);
        return false;
    }    
    
    template <typename T> 
    inline T BuildEntity(const nlohmann::json& j){
        T c{};
        if constexpr (std::is_same_v<T, Character>){
            JsonModify::ModifyCharacter(j,c);
        }else if constexpr (std::is_same_v<T, CursedTool>) {
            JsonModify::ModifyCursedTool(j, c);
        }else if constexpr (std::is_same_v<T, Technique>){
            JsonModify::ModifyTechnique(j, c);
        }else if constexpr (std::is_same_v<T, Domain>){
            JsonModify::ModifyDomain(j, c);
        }else if constexpr (std::is_same_v<T, Neutralizer>){
            JsonModify::ModifyNeutralizer(j, c);
        }else if constexpr (std::is_same_v<T, Shikigami>){
            JsonModify::ModifyShikigami(j, c);
        }else if constexpr (std::is_same_v<T, StatusEffect>){
            JsonModify::ModifyStatusEffect(j, c);
        }
        return c;
    }
    template<typename T>
    inline std::vector<T> LoadEntity() {
        std::string fl{}, js{};
        
        if constexpr (std::is_same_v<T, Character>) {
            fl = "Characters.json";
            js = "Characters";
        }else if constexpr (std::is_same_v<T, CursedTool>){
            fl = "CursedTools.json";
            js = "Cursed Tools";
        }else if constexpr (std::is_same_v<T, StatusEffect>){
            fl = "StatusEffects.json";
            js = "Status Effects";
        }else if constexpr (std::is_same_v<T, Domain>){
            fl = "Domains.json";
            js = "Domains";
        }else if constexpr (std::is_same_v<T, Neutralizer>){
            fl = "Neutralizers.json";
            js = "Neutralizers";
        }else if constexpr (std::is_same_v<T, Technique>){
            fl = "Techniques.json";
            js = "Techniques";
        }else if constexpr (std::is_same_v<T, Shikigami>){
            fl = "Shikigami.json";
            js = "Shikigami";
        }else{
            static_assert(false, "Unsupported JSON entity type!");
        }

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
        std::vector<T> v;
        
        if (data.contains(js) && data[js].is_array()) {
            v.reserve(data[js].size());
            for (const auto& charData : data[js]) {
                v.push_back(Json::BuildEntity<T>(charData));
            }
        }
        return v;
    }
}

template <typename T>
using UMap = std::unordered_map<std::string, T>;

namespace UMaps {
    UMap<CursedTool> GetCTMap();
    UMap<Shikigami> GetSKMap();
    UMap<Technique> GetTCMap();
    UMap<Domain> GetDMMap();
    UMap<Neutralizer> GetNLMap();
    UMap<StatusEffect> GetSTMap();
}