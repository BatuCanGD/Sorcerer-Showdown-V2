#pragma once
#include "AI.hpp"
#include "CharacterType/CurseUser.hpp"
#include <nlohmann/json.hpp>
#include <memory>
#include <string_view>
#include <optional>

class Character;
class CurseUser;

struct Shikigami;
struct CursedTool;
struct Technique;
struct Domain;
struct Neutralizer;

namespace Json {
    void LoadCharacters(std::vector<std::unique_ptr<Character>>& bf);
    [[nodiscard]] std::unique_ptr<Character> BuildCharacter(const nlohmann::json& j);

    void GetDefaultValues(const nlohmann::json& j, Character& c);
    void GetCurseUserValues(const nlohmann::json& j, CurseUser& c);
    void ModifyInventory(const nlohmann::json& j, std::vector<CursedTool>& t);
    void ModifyShikigami(const nlohmann::json& j, std::vector<Shikigami>& t);

    FightingStyle GetFightingStyle(std::string_view sv);
    ResourceUsage GetResourceUsage(std::string_view sv);
    TargetingType GetTargetingType(std::string_view sv);
    TargetedTypes GetTargetedTypes(std::string_view sv);
    CursedEnergySystem::Efficiency GetEfficiency(std::string_view sv);

    std::optional<CursedTool> GetCursedTool(std::string_view sv);
    std::optional<Neutralizer> GetNeutralizer(std::string_view sv);
    std::optional<Technique> GetTechnique(std::string_view sv);
    std::optional<Domain> GetDomain(std::string_view sv);
}