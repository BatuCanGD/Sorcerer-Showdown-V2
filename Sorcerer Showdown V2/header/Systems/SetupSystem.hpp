#pragma once
#include <cstdint>
#include <map>
#include <string>

struct Battlefield;
class Character;

enum class SkipType : std::uint8_t {
    None,
    Turns,
    All
};

namespace SetupSystem {
    SkipType GetPlayerSkipType() noexcept;
    Character* SetupLoop(Battlefield& bf);
    void LogSetupOptions();
    bool SetupOptions(Battlefield& bf, Character*& c);
    void BattlefieldSetup(Battlefield& bf);
    std::map<std::string, int> SetList(const Battlefield& bf);
}