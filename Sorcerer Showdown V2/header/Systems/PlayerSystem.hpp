#pragma once
#include <cstdint>
#include <vector>

struct Battlefield;
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
    std::vector<Action> GetChoices(const Character& c);
    void GetPlayerTurn(Character& c, Battlefield& bf);

    Character* GetUserTarget(Battlefield& bf);

    bool DoAttack(Character& c, Character& cd);
    bool DoInventoryManagement(Character& c);

    bool DoSorcery(CurseUser* c);
    bool DoTechnique(CurseUser* c, Character& cd);
    bool DoDomain(CurseUser* c);
    bool DoShikigami(CurseUser* c);
}