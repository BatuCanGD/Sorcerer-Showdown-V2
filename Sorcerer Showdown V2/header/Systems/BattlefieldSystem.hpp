#pragma once

class Character;
struct Battlefield;

namespace BattlefieldSystem {
    void HandleDeadPeople(Battlefield& bf);
    void HandleSpawns(Battlefield& bf);
};