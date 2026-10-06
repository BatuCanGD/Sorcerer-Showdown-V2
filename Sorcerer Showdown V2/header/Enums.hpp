#pragma once
#include <cstdint>
namespace globalums // enums that are used by multiple systems
{
    enum class DamageType : std::uint8_t {
        Normal, // can be stopped by barriers or negated by reinforcement
        BypassTech, // technique barriers
        BypassRein, // Cursed Energy Reinforcement
        BypassAll // both bypasses apply
    };
}