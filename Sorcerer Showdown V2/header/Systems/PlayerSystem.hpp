#pragma once
#include <cstdint>
#include <utility>
#include <variant>
#include <string>
#include <vector>

enum class SummonType : std::uint8_t;
struct Battlefield;
struct Shikigami;
struct Character;


enum class Action : std::uint8_t {
    Attack,
    Technique,
    Domain,
    Shikigami,
    Inventory,
    Effects,
    Sorcery // RCT, Reinforcement, Binding Vows
};

namespace UserControl {
    std::pair<std::vector<Action>, std::vector<std::string>> GetChoices(const Character& c);
    void GetPlayerTurn(Character& c, Battlefield& bf);
    bool GetConfirmation();

    std::variant<Character*, Shikigami*> GetTarget(const Character& c, Battlefield& bf);
    Character* GetCharacterTarget(const Character& c, Battlefield& bf);

    void DoAttack(Character& c, std::variant<Character*, Shikigami*> cd);
    void DoInventoryManagement(Character& c);
    void DoEffects(const Character& c);

    enum class SorceryType : std::uint8_t { RCT, Reinforcement, BindingVows, Amplification};
    void DoSorcery(Character& c);
    const std::vector<SorceryType> GetSorceryChoices(const Character& c);
    void ForAmplification(Character& c);
    void ForRCT(Character& c);
    void ForReinforcement(Character& c);
    void ForBindingVows(Character& c);

    void DoTechnique(Character& c, Battlefield& bf);
    void DoDomain(Character& c);

    void DoShikigami(Character& c);
    Shikigami* ChooseShikigami(std::vector<Shikigami>& sh);
    void UseShikigami(Shikigami& c);

}