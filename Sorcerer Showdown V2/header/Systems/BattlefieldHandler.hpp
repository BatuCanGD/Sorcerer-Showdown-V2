#pragma once

#include <unordered_map>
#include <memory>

class Character;
struct Battlefield;

class BattlefieldHandler final {
    static void PrintSetupValues(const std::unordered_map<int, std::unique_ptr<Character>>& list);
    static bool ManageSetupChoices(int x);
public:
    static void SetupBattlefield(Battlefield& bf);
    static void HandleDeadPeople(Battlefield& bf);
};