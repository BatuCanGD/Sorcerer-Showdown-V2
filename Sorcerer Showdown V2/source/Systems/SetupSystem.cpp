#include "../../header/Systems/SetupSystem.hpp"
#include "../../header/CharacterType/Character.hpp"
#include "../../header/Battlefield.hpp"
#include "../../header/Utilities/Input.hpp"

#include <print>
#include <map>

SkipType SetupSystem::GetPlayerSkipType() noexcept {
    std::println("1 - Skip Everything\n2 - Skip to end of round\n3 - Dont skip");
    int tp = get_input<int>();
    if (tp == 1){
        return SkipType::All;
    } else if (tp == 2){
        return SkipType::Turns;
    } else {
        return SkipType::None;
    }
}

std::map<std::string, int> SetupSystem::SetList(const Battlefield &bf){
    std::map<std::string, int> list;
    for (const auto& c : bf.battlefield){
        list[c->Name()]++;
    }
    return list;
}

Character* SetupSystem::SetupLoop(Battlefield& bf) {
    Character* c{nullptr};
    bool keep_looping = true;
    while(keep_looping){
        for (const auto& [name, num] : SetupSystem::SetList(bf)){
            std::println("{}x {}", num, name);
        }
        SetupSystem::LogSetupOptions();
        keep_looping = SetupSystem::SetupOptions(bf, c);
    }
    return c;
}


void SetupSystem::LogSetupOptions() {
    std::println("1 - Add Character | 11 - Add multiple characters\n",
                 "2 - Remove Character | 22 - Remove last character\n",
                 "3 - Clear Battlefield | 33 - Additional Character Info");
}

bool SetupSystem::SetupOptions(Battlefield& bf, Character*& c) {

    return true;
}