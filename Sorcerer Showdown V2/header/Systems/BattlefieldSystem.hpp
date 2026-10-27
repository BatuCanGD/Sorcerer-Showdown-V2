#pragma once

struct Character;
struct Battlefield;

namespace BattlefieldSystem {
    void HandleDomainInteraction(Battlefield& bf);
    void HandleDeadPeople(Battlefield& bf);
    void HandleDeadShikigami(Battlefield& bf);
    void HandleSpawns(Battlefield& bf);
    bool CanContinueFight(Battlefield& bf);
};