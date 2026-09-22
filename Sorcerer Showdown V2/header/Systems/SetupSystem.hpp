#pragma once
#include <cstdint>
#include <vector>
#include <memory>
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

    void AddCharacter(Battlefield& bf, Character*& c);
    void AddCharacters(Battlefield& bf);
    void RemoveCharacter(Battlefield& bf, Character*& c);
    void RemoveLast(Battlefield& bf, Character*& c);
    void ClearBattlefield(Battlefield& bf, Character*& c);
    void ViewCharacterInfo(Battlefield& bf);

    const std::vector<std::unique_ptr<Character>> GetCharacterList();
    const std::map<std::string, int> SetList(const Battlefield& bf);
}