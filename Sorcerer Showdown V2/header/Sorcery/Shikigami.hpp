#pragma once
#include "../Info.hpp"
#include "../Structs.hpp"

#include <cstdint>

class Character;

enum class SupportType : std::uint8_t {
    Offense,
    Defense,
    Output,
    CursedEnergyRegen
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
    SavedValue rollback;
    double cost{1.0};
    SummonType summon_type{SummonType::Shadow};
    SupportType support_type{SupportType::Offense};
};