#pragma once
#include <cstdint>
#include <map>
#include <string>

struct Battlefield;

enum class SkipType : std::uint8_t {
    None,
    Turns,
    All
};

namespace GameEngine {
    SkipType GetPlayerSkipType() noexcept;
    bool SetupLoop(Battlefield& bf);
    void BattlefieldSetup(Battlefield& bf);
    std::map<std::string, int> SetList(const Battlefield& bf);
}