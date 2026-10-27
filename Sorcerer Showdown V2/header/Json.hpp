#pragma once
#include "AI.hpp"
#include "General/StatusEffects.hpp"
#include "Sorcery/Domain.hpp"
#include "Sorcery/Shikigami.hpp"
#include "SorceryStruct.hpp"
#include <nlohmann/json.hpp>
#include <memory>
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

namespace Json {
    std::vector<std::unique_ptr<Character>> LoadCharacters();
    std::vector<CursedTool> LoadCursedTools();
    std::vector<Technique> LoadTechniques();
    std::vector<Domain> LoadDomains();
    std::vector<Shikigami> LoadShikigami();
    std::vector<StatusEffect> LoadEffects();

    [[nodiscard]] std::unique_ptr<Character> BuildCharacter(const nlohmann::json& j);
    [[nodiscard]] CursedTool BuildTool(const nlohmann::json& j);
    [[nodiscard]] Technique BuildTechnique(const nlohmann::json& j);
    [[nodiscard]] Domain BuildDomain(const nlohmann::json& j);
    [[nodiscard]] Shikigami BuildShikigami(const nlohmann::json& j);
    [[nodiscard]] StatusEffect BuildEffect(const nlohmann::json& j);

    void GetBaseValues(const nlohmann::json& j, Character& c);
    void GetSorceryValues(const nlohmann::json& j, Sorcery::CurseUserSystem& c);
    void ModifyInventory(const nlohmann::json& j, std::vector<CursedTool>& t);
    void ModifyShikigami(const nlohmann::json& j, std::vector<Shikigami>& t);

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