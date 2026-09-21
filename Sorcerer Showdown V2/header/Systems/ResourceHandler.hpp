#pragma once
#include <vector>

class Character;
class CurseUser;

struct StatusEffect;

namespace ResourceHandler {
    void TickStatusEffects(std::vector<StatusEffect>& character);
    void TickCursedEnergy(CurseUser& curse_user);
    void TickShikigami(CurseUser& curse_user);
    void TickRCT(CurseUser& sorcerer);
    void SpendNeutralizerCost(CurseUser& curse_user);
    void SpendDomainCost(CurseUser& curse_user);
}