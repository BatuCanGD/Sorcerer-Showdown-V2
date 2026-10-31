#pragma once
#include <optional>
struct Character;
struct Battlefield;
struct StatusEffect;

namespace Sorcery {
    struct CurseUserSystem;
}

namespace ResourceHandler {
    void TickAll(Battlefield& bf);
    void TickStun(Character& c);
    void TickTraits(Character& c);
    void TickShikigami(Character& c);
    void TickChant(Character& c);
    void TickRCT(Character& c);

    void TickReinforcement(std::optional<Sorcery::CurseUserSystem>& c);
    void TickCursedEnergy(std::optional<Sorcery::CurseUserSystem>& c);
    void TickTechBarrier(std::optional<Sorcery::CurseUserSystem>& c);
    void TickAmp(std::optional<Sorcery::CurseUserSystem>& c);
    void TickOutputStatus(std::optional<Sorcery::CurseUserSystem>& c);
    void TickOutput(std::optional<Sorcery::CurseUserSystem>& c);
}