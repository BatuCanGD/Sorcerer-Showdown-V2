#pragma once
#include <cstdint>

enum class OpType : std::uint8_t { Get, Set, Add, Expend };
enum class ValType : std::uint8_t { Current, Maximum };

namespace globalums // enums that are used by multiple systems
{
    enum class DamageType : std::uint8_t {
        Normal, // can be stopped or negated by reinforcement
        BypassTech, // technique barriers
        BypassRein, // Cursed Energy Reinforcement
        BypassAll // both bypasses apply
    };
}