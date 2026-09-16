#pragma once
#include "../Structs.hpp"

#include <cstdint>

class Character;
class CurseUser;

enum class SupportType : std::uint8_t {
    Offense,
    Boost,
    Protection,
    Regeneration
};

enum class SummonType : std::uint8_t {
    Shadow,     // disabled       | not vulnerable
    Support,    // halved support | |
    Active      // full support   | vulnerable
};

struct HealthValues final {
    double health{1.0};
    double max_health{1.0};
    double regen_speed{1.0};
};

struct ShikigamiValues final {
    double cost{1.0};
    double support_value{1.0}; // provides X amount of support/damage/boost
};

struct ShikigamiType final {
    SummonType summon_type{SummonType::Shadow};
    SupportType support_type{SupportType::Offense};
};

struct Shikigami final {
    EntityInfo id;
    HealthValues hp;
    Character* target{nullptr}; // focuses support on the target whether it be its owner or an enemy
    ShikigamiValues val;
    ShikigamiType types;
};