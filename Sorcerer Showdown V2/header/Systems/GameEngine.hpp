#pragma once
#include <cstdint>

struct Battlefield;

enum class SkipType : std::uint8_t {
    None,
    Turns,
    All
};

namespace GameEngine {
    SkipType GetPlayerSkipType() noexcept;
    bool PlayerSetupBattlefield(Battlefield& bf);
    void Placeholder3();
}