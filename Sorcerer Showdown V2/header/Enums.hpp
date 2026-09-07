#pragma once

#include <cstdint>

namespace type {
    enum class Type : std::uint8_t { Get, Set, Add, Expend };

    enum class Get : std::uint8_t { Current, Max };
    enum class Set : std::uint8_t { Current, Max };
    enum class Expend : std::uint8_t { Current, Max };
    enum class Add    : std::uint8_t { Current, Max };
}

namespace charenums // character enums
{ 
    enum class NameType : std::uint8_t {
        Name,
        Color,
        Both
    };
}

namespace globalums // enums that are used by multiple systems
{
    enum class DamageType : std::uint8_t {
        Normal, // can be stopped or negated by reinforcement
        BypassTech, // techniques, infinity, etc...
        BypassRein, // Cursed Energy Reinforcement
        BypassAll // both bypasses apply
    };
}