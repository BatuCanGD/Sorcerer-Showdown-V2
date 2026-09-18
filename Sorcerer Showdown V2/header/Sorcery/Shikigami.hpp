#pragma once
#include "../Structs.hpp"

#include <cstdint>

class Character;

enum class SupportType : std::uint8_t {
    Offense,
    Defense,
    Output,
    CursedEnergyRegen
};

struct SavedSupportValues final {
    double s_strength{};
    double s_durability{};
    double s_output{};
    double s_ce_regen{};
    double s_hp_regen{};
    bool saved{};
    bool undone{};
};

enum class SummonType : std::uint8_t {
    Shadow,     // disabled       | not vulnerable
    Support,    // halved support | |
    Active      // full support   | vulnerable
};

struct ShikigamiHealth final {
    double health{1.0};
    double max_health{1.0};
    double regen_speed{1.0};
};

struct Shikigami final {
    EntityInfo id;
    ShikigamiHealth hp;
    SavedSupportValues saved_val;
    double cost{1.0};
    SummonType summon_type{SummonType::Shadow};
    SupportType support_type{SupportType::Offense};
};