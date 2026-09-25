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
        list[c->Name()]++;
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
        for (const auto& [name, num] : SetupSystem::SetList(bf)){
            std::println("{}x {}", num, name);
        }
        SetupSystem::LogSetupOptions();
        looping = SetupSystem::SetupOptions(bf, c);
    }
    return c;
}


void SetupSystem::LogSetupOptions() {
    std::println("1 - Add Character | 11 - Add multiple characters\n"
                 "2 - Remove Character | 22 - Remove last character\n"
                 "3 - Clear Battlefield | 33 - Additional Character Info\n"
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
        case 2:
            SetupSystem::RemoveCharacter(bf, c);
            break;
        case 22:
            SetupSystem::RemoveLast(bf, c);
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

void SetupSystem::AddCharacter(Battlefield& bf, Character*& c){
    int pc{0};
    if (c == nullptr){
        std::println("Would you like to set your character\n1 - Yes | 2 - No");
        pc = get_input<int>();
    }
    size_t x{0};
    const auto list = SetupSystem::GetCharacterList();
    for (const auto& z : list) {
        std::println("{}:{}", ++x, z->Name());
    }
    x = get_input<size_t>();
    if (x == 0 || x > list.size()) {
        return;
    }
    bf.battlefield.push_back(list[--x]->Clone());
    if (pc == 1) {
        c = bf.battlefield.back().get();
    }
}
void SetupSystem::AddCharacters(Battlefield& bf){
    std::println("How much of a character would you like to add\n=> ");
    int x = get_input<int>();
    size_t g{0};
    const auto list = SetupSystem::GetCharacterList();
    for (const auto& z : list){
        std::println("{}:{}", ++g, z->Name());
    }
    std::println("Which character would you like to add");
    g = get_input<size_t>();
    if (g == 0 || g > list.size()){
        return;
    }
    --g;
    for (int i = 0; i < x; i++){
        bf.battlefield.push_back(list[g]->Clone());
    }
}

void SetupSystem::RemoveCharacter(Battlefield& bf, Character*& c){
    if (bf.battlefield.empty()) return;
    size_t x{0};
    for (const auto& b : bf.battlefield){
        std::println("{}:{}", ++x, b->Name());
    }
    x = get_input<size_t>();
    if (x == 0 || x > bf.battlefield.size()) {
        return;
    } 
    --x;
    if (bf.battlefield[x].get() == c) c = nullptr;
    bf.battlefield.erase(bf.battlefield.begin() + x);
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
        std::println("{}:{}", ++x, c->Name());
    }
    std::println("Who's info would you like to view");
    x = get_input<size_t>();
    if (x == 0 || x > bf.battlefield.size()){
        return;
    }
    Log::CharacterInfo(*bf.battlefield[--x]);
}