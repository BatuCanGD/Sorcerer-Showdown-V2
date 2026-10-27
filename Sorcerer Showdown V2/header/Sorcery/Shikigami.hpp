#pragma once
#include "../Info.hpp"

#include <cstdint>

struct Character;

enum class SupportType : std::uint8_t {
    None,
    Offense,
    Defense,
    Output,
    CursedEnergyRegen
};
enum class ActiveAbility : std::uint8_t {
    None,
    TechniqueBypass
};

enum class SummonType : std::uint8_t {
    Shadow,     // disabled       | not vulnerable
    Support,    // halved support | |
    Active      // full support   | vulnerable
};

struct ShikigamiStats final {
    double health{1.0};
    double max_health{1.0};
    double regen_speed{1.0};
    double durability{1.0};
};

struct SavedValue final {
    double value{};
    bool saved{};
    bool undone{};
};

struct Shikigami final {
    EntityInfo id;
    ShikigamiStats stats;
    SavedValue rollback;
    double cost{1.0};
    SummonType summon_type{SummonType::Shadow};
    ActiveAbility active_ability{ActiveAbility::None};
    SupportType support_type{SupportType::None};
};