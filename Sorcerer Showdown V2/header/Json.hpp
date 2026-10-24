#pragma once
#include "AI.hpp"
#include <nlohmann/json.hpp>
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
    void GetDefaultValues(const nlohmann::json& j, Character& c);
    void GetCurseUserValues(const nlohmann::json& j, CurseUser& c);
    
    void InsertCursedTools(const nlohmann::json& j, std::string_view sv, Character& c);

    FightingStyle GetFightingStyle(std::string_view sv);
    ResourceUsage GetResourceUsage(std::string_view sv);
    TargetingType GetTargetingType(std::string_view sv);
    TargetedTypes GetTargetedTypes(std::string_view sv);

    std::optional<Neutralizer> GetNeutralizer(std::string_view sv);
    std::optional<Shikigami> GetShikigami(std::string_view sv);
    std::optional<Technique> GetTechnique(std::string_view sv);
    std::optional<Domain> GetDomain(std::string_view sv);
}