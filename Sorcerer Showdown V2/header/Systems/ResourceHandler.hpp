#pragma once

class Character;
class CurseUser;
struct Battlefield;
struct StatusEffect;

namespace ResourceHandler {
    void TickAll(Battlefield& bf);
    void TickTraits(CurseUser& curse_user);
    void TickCursedEnergy(CurseUser& curse_user);
    void TickShikigami(CurseUser& curse_user);
    void TickRCT(CurseUser& curse_user);
    void TickOutputStatus(CurseUser& curse_user);
    void TickOutput(CurseUser& curse_user);
}