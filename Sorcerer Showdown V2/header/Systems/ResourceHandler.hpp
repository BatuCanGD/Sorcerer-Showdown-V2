#pragma once

class Character;
class CurseUser;
struct Battlefield;
struct StatusEffect;

namespace ResourceHandler {
    void TickAll(Battlefield& bf);
    void TickCursedEnergy(CurseUser& curse_user);
    void TickShikigami(CurseUser& curse_user);
    void TickRCT(CurseUser& sorcerer);
}