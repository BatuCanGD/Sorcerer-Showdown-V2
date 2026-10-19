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

    enum class SetupOpt : std::uint8_t { Start, AddChar, AddMult, Choose, RemoveSpecific, RemoveLast, ChooseBf,  Clear, CharInfo, Json };

    Character* SetupLoop(Battlefield& bf);
    std::vector<SetupOpt> GetSetupOptions();
    bool SetupOptions(const std::vector<SetupOpt>& opts, Battlefield& bf, Character*& c, bool& load_json);
    void BattlefieldSetup(Battlefield& bf);

    void AddCharacter(Battlefield& bf, Character*& c, bool choosing, bool load_json);
    void AddCharacters(Battlefield& bf, bool load_json);
    void RemoveCharacter(Battlefield& bf, Character*& c, bool choosing); // used for choosing character aswell
    void RemoveLast(Battlefield& bf, Character*& c);
    void ClearBattlefield(Battlefield& bf, Character*& c);
    void ViewCharacterInfo(Battlefield& bf);

    const std::vector<std::unique_ptr<Character>> GetCharacterList(bool get_custom);
    const std::map<std::string, int> SetList(const Battlefield& bf);
}