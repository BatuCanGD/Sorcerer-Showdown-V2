#pragma once
#include "AI.hpp"
#include "SorceryStruct.hpp"
#include <nlohmann/json.hpp>
#include <memory>
#include <string_view>
#include <optional>

struct Character;


struct Shikigami;
struct CursedTool;
struct Technique;
struct Domain;
struct Neutralizer;

namespace Json {
    std::vector<std::unique_ptr<Character>> LoadCharacters();
    std::vector<CursedTool> LoadCursedTools();
    std::vector<Technique> LoadTechniques();
    std::vector<Domain> LoadDomains();
    std::vector<Shikigami> LoadShikigami();

    [[nodiscard]] std::unique_ptr<Character> BuildCharacter(const nlohmann::json& j);
    [[nodiscard]] CursedTool BuildTool(const nlohmann::json& j);
    [[nodiscard]] Technique BuildTechnique(const nlohmann::json& j);
    [[nodiscard]] Domain BuildDomain(const nlohmann::json& j);
    [[nodiscard]] Shikigami BuildShikigami(const nlohmann::json& j);

    void GetDefaultValues(const nlohmann::json& j, Character& c);
    void GetCurseUserValues(const nlohmann::json& j, Sorcery::CurseUserSystem& c);
    void ModifyInventory(const nlohmann::json& j, std::vector<CursedTool>& t);
    void ModifyShikigami(const nlohmann::json& j, std::vector<Shikigami>& t);

    FightingStyle GetFightingStyle(std::string_view sv);
    ResourceUsage GetResourceUsage(std::string_view sv);
    TargetingType GetTargetingType(std::string_view sv);
    TargetedTypes GetTargetedTypes(std::string_view sv);
    Sorcery::CursedEnergySystem::Efficiency GetEfficiency(std::string_view sv);

    std::optional<CursedTool> GetCursedTool(std::string_view sv);
    std::optional<Neutralizer> GetNeutralizer(std::string_view sv);
    std::optional<Technique> GetTechnique(std::string_view sv);
    std::optional<Domain> GetDomain(std::string_view sv);
}