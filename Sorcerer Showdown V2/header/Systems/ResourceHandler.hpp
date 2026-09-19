#pragma once

class Character;
class CurseUser;

namespace ResourceHandler {
    void TickStatusEffects(Character& character);
    void TickCursedEnergy(CurseUser& curse_user);
    void TickShikigami(CurseUser& curse_user);
    void TickNeutralizer(CurseUser& curse_user);
    void TickDomain(CurseUser& curse_user);
    void TickRCT(CurseUser& sorcerer);
}