#pragma once

struct Character;

struct Battlefield;
struct StatusEffect;

namespace ResourceHandler {
    void TickAll(Battlefield& bf);
    void TickTraits(Character& curse_user);
    void TickCursedEnergy(Character& curse_user);
    void TickShikigami(Character& curse_user);
    void TickChant(Character& curse_user);
    void TickAmp(Character& curse_user);
    void TickRCT(Character& curse_user);
    void TickOutputStatus(Character& curse_user);
    void TickOutput(Character& curse_user);
}