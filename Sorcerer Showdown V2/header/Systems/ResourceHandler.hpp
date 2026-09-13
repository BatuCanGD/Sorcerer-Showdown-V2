#pragma once

class Sorcerer;
class CurseUser;

namespace ResourceHandler {
    void TickCursedEnergy(CurseUser& curse_user);
    void TickShikigami(CurseUser& curse_user);
    void UsedNeutralizer(CurseUser& curse_user);
    void UsedDomain(CurseUser& curse_user);
    void TickReverseCursedTechnique(Sorcerer& sorcerer);
}