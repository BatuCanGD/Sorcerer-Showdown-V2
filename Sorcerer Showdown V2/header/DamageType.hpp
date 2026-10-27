#pragma once
#include <cstdint>

enum class DamageType : std::uint8_t {
    Normal, // can be stopped by barriers or negated by reinforcement
    BypassTech, // technique barriers
    BypassRein, // Cursed Energy Reinforcement
    BypassAll // both bypasses apply
};

namespace DamageCheck {
    bool BypassesTech(DamageType dt);
    bool BypassesReinforcement(DamageType dt);
}