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
    bool SetupOptions(Battlefield& bf, Character*& c, bool& load_json);
    void BattlefieldSetup(Battlefield& bf);

    void AddCharacter(Battlefield& bf, Character*& c, bool choosing, bool load_json);
    void AddCharacters(Battlefield& bf, bool load_json);
    void RemoveCharacter(Battlefield& bf, Character*& c, bool choosing); // used for choosing character aswell
    void RemoveLast(Battlefield& bf, Character*& c);
    void ClearBattlefield(Battlefield& bf, Character*& c);
    void ViewCharacterInfo(Battlefield& bf);

    std::vector<std::unique_ptr<Character>> GetCustomCharacters();
    const std::vector<std::unique_ptr<Character>> GetCharacterList(bool get_custom);
    const std::map<std::string, int> SetList(const Battlefield& bf);
}