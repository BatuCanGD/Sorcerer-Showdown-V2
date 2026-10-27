#pragma once
#include "AIEnums.hpp"
#include "General/CursedTool.hpp"
#include <vector>
#include <optional>

struct BindingVow;
struct Shikigami;
struct Technique;
struct Domain;
struct Neutralizer;

namespace Base {
    struct DecisionMaker final {
        TargetingType targeting_type{TargetingType::Random};
        TargetedTypes targeted_type {TargetedTypes::Mixed};
        FightingStyle fighting_style{FightingStyle::Mixed};
        ResourceUsage resource_usage{ResourceUsage::Mixed};
    };
    struct State final {
        std::vector<StatusEffect> status_effects;
        double health{1.0};
        double max_health{1.0};
        double durability{1.0};
        double strength{1.0};
        bool is_stunned{false};
        bool is_invulnerable{false};
    };
    struct Equipment final {
        std::vector<CursedTool>   inventory;
        std::optional<CursedTool> stored_tool;
        std::optional<CursedTool> current_tool;
        bool has_access_to_inventory{false};
    };
}