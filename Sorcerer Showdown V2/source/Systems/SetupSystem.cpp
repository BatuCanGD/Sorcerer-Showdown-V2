#include "../../header/Systems/SetupSystem.hpp"
#include "../../header/CharacterType/CurseUser.hpp"
#include "../../header/Battlefield.hpp"
#include "../../header/Utilities/Input.hpp"
#include "../../header/Creator.hpp"
#include "../../header/Logger.hpp"

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

const std::map<std::string, int> SetupSystem::SetList(const Battlefield &bf){
    std::map<std::string, int> list;
    for (const auto& c : bf.battlefield){
        list[GetInfo::Name(c->Identity())]++;
    }
    return list;
}

const std::vector<std::unique_ptr<Character>> SetupSystem::GetCharacterList() {
    std::vector<std::unique_ptr<Character>> list;
    list.push_back(Create::Gojo());
    list.push_back(Create::Mahito());
    return list;
}

Character* SetupSystem::SetupLoop(Battlefield& bf) {
    Character* c{nullptr};
    bool looping{true};
    while(looping){
        std::println("Chosen: [{}]", c ? GetInfo::Name(c->Identity()) : "None");
        for (const auto& [name, num] : SetupSystem::SetList(bf)){
            std::println("{}x {}", num, name);
        }
        SetupSystem::LogSetupOptions();
        looping = SetupSystem::SetupOptions(bf, c);
    }
    return c;
}


void SetupSystem::LogSetupOptions() {
    std::println("1 - Add Character         | 11 - Add multiple characters         | 111 - Choose Character\n"
                 "2 - Remove Character      | 22 - Remove last character          |  222 - Choose from battlefield\n"
                 "3 - Clear Battlefield     | 33 - Additional Character Info\n"
                 "0 - Start Game");
}

bool SetupSystem::SetupOptions(Battlefield& bf, Character*& c) {
    switch(get_input<int>()){
        case 1:
            SetupSystem::AddCharacter(bf, c);
            break;
        case 11:
            SetupSystem::AddCharacters(bf);
            break;
        case 111:
            SetupSystem::AddCharacter(bf, c, true);
            break;
        case 2:
            SetupSystem::RemoveCharacter(bf, c);
            break;
        case 22:
            SetupSystem::RemoveLast(bf, c);
            break;
        case 222:
            SetupSystem::RemoveCharacter(bf, c, true);
            break;
        case 3:
            SetupSystem::ClearBattlefield(bf, c);
            break;
        case 33:
            SetupSystem::ViewCharacterInfo(bf);
            break;
        case 0:
            if (bf.battlefield.size() < 2){
                std::println("Not enough fighters");
                return true;
            }
            return false;
        default:
            std::println("Invalid Input");
            break;
    }
    return true;
}

void SetupSystem::AddCharacter(Battlefield& bf, Character*& c, bool p_choosing){
    size_t x{0};
    const auto list = SetupSystem::GetCharacterList();
    for (const auto& z : list) {
        std::println("{}:{}", ++x, GetInfo::Name(z->Identity()));
    }
    x = get_input<size_t>() - 1;
    if (x >= list.size()) {
        return;
    }
    bf.battlefield.push_back(list[x]->Clone());
    if (p_choosing) {
        c = bf.battlefield.back().get();
    }
}
void SetupSystem::AddCharacters(Battlefield& bf){
    size_t g{0};
    const auto list = SetupSystem::GetCharacterList();
    for (const auto& z : list){
        std::println("{}:{}", ++g, GetInfo::Name(z->Identity()));
    }
    std::println("Which character would you like to add");
    g = get_input<size_t>() - 1;
    if (g >= list.size()){
        return;
    }
    std::println("How many?\n=> ");
    int x = get_input<int>();
    for (int i = 0; i < x; i++){
        bf.battlefield.push_back(list[g]->Clone());
    }
}

void SetupSystem::RemoveCharacter(Battlefield& bf, Character*& c, bool p_choose){
    if (bf.battlefield.empty()) return;
    size_t x{0};
    for (const auto& b : bf.battlefield){
        std::println("{}:{}", ++x, GetInfo::Name(b->Identity()));
    }
    x = get_input<size_t>() - 1;
    if (x >= bf.battlefield.size()) {
        return;
    } 
    if (p_choose){
        c = bf.battlefield[x].get();
    }else{
        if (bf.battlefield[x].get() == c) c = nullptr;
        bf.battlefield.erase(bf.battlefield.begin() + x);
    }
}
void SetupSystem::RemoveLast(Battlefield& bf, Character*& c) {
    if (bf.battlefield.empty()) return;
    if (bf.battlefield.back().get() == c) c = nullptr;
    bf.battlefield.pop_back();
}
void SetupSystem::ClearBattlefield(Battlefield& bf, Character*& c){
    if (bf.battlefield.empty()) return;
    bf.battlefield.clear();
    c = nullptr;
}
void SetupSystem::ViewCharacterInfo(Battlefield &bf){
    if (bf.battlefield.empty()) return;
    size_t x{0};
    for (const auto& c : bf.battlefield){
        std::println("{}:{}", ++x, GetInfo::Name(c->Identity()));
    }
    std::println("Who's info would you like to view");
    x = get_input<size_t>();
    if (x == 0 || x > bf.battlefield.size()){
        return;
    }
    Log::CharacterPresentation(*bf.battlefield[--x]);
}