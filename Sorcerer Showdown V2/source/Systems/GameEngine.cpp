#include "../../header/Systems/GameEngine.hpp"
#include "../../header/CharacterType/Character.hpp"
#include "../../header/Battlefield.hpp"
#include "../../header/Utilities/Input.hpp"

#include <print>
#include <map>

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

std::map<std::string, int> GameEngine::SetList(const Battlefield &bf){
    std::map<std::string, int> list;
    for (const auto& c : bf.battlefield){
        list[c->Name()]++;
    }
    return list;
}

bool GameEngine::SetupLoop(Battlefield& bf) {
    Character* c{nullptr};
    while(true){
        for (const auto& [name, num] : GameEngine::SetList(bf)){
            std::println("{}x {}", num, name);
        }
    }
    return c != nullptr;
}