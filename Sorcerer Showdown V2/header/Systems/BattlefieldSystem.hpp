#pragma once

class Character;
struct Battlefield;

namespace BattlefieldSystem {
    void HandleDomainInteraction(Battlefield& bf);
    void HandleDeadPeople(Battlefield& bf);
    void HandleSpawns(Battlefield& bf);
};