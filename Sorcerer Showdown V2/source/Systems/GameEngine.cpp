#include "../../header/Systems/GameEngine.hpp"
#include "../../header/CharacterType/CurseUser.hpp"
#include "../../header/Battlefield.hpp"
#include "../../header/Utilities/Input.hpp"

#include <print>

SkipType GameEngine::GetPlayerSkipType() noexcept {
    std::println("1 - Skip Everything\n2 - Skip to end of round\n3 - Dont skip");
    int tp = get_input<int>();
    SkipType st;
    if (tp == 1){
        st = SkipType::All;
    } else if (tp == 2){
        st = SkipType::Turns;
    } else {
        st = SkipType::None;
    }
    return st;
}

bool GameEngine::PlayerSetupBattlefield(Battlefield& bf) {
    Character* c{nullptr};
    while(true){
        std::println("Placeholder PlayerSetupBattlefield println");
    }
    return c == nullptr;
}