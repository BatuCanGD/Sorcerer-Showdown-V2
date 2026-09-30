#pragma once
#include <cstdint>
#include <utility>
#include <string>
#include <vector>

enum class SummonType : std::uint8_t;
struct Battlefield;
struct Shikigami;
class Character;
class CurseUser;

enum class Action : std::uint8_t {
    Attack,
    Technique,
    Domain,
    Shikigami,
    Inventory,
    Sorcery // RCT, Reinforcement, Binding Vows
};

namespace UserControl {
    std::pair<std::vector<Action>, std::vector<std::string>> GetChoices(const Character& c);
    void GetPlayerTurn(Character& c, Battlefield& bf);
    bool GetConfirmation();

    Character* GetUserTarget(Battlefield& bf);

    bool DoAttack(Character& c, Character& cd);
    bool DoInventoryManagement(Character& c);


    enum class SorceryType : std::uint8_t { RCT, Reinforcement, BindingVows };
    bool DoSorcery(CurseUser* c);
    const std::vector<SorceryType> GetSorceryChoices(const CurseUser& c);
    void ForRCT(CurseUser& c);
    void ForReinforcement(CurseUser& c);
    void ForBindingVows(CurseUser& c);

    bool DoTechnique(CurseUser* c, Character& cd);
    bool DoDomain(CurseUser* c);

    bool DoShikigami(CurseUser* c);
    Shikigami* ChooseShikigami(std::vector<Shikigami>& sh);
    void UseShikigami(Shikigami& c);

}