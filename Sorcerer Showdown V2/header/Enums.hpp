#pragma once

#include <cstdint>

namespace type {
    enum class Get : std::uint8_t { Current, Max, Previous };
    enum class Set : std::uint8_t { Current, Max, Previous };
}

namespace charenums // character enum type
{ 
    enum class NameType : std::uint8_t {
        Name,
        Color,
        Both
    };
}
namespace globalums
{
    enum class DamageType : std::uint8_t {
        Normal, // can be stopped or negated by reinforcement
        BypassTech, // techniques, infinity, etc...
        BypassRein, // Cursed Energy Reinforcement
        BypassAll // both bypasses apply
    };
}