#include "../../header/Systems/SetupSystem.hpp"
#include "../../header/CharacterType/CurseUser.hpp"
#include "../../header/Battlefield.hpp"
#include "../../header/Utilities/Input.hpp"
#include "../../header/Creator.hpp"
#include "../../header/Logger.hpp"

#include <print>
#include <fstream>
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
std::vector<std::unique_ptr<Character>> SetupSystem::GetCustomCharacters() {
    std::println(stderr, "Looking for characters.json in: {}", std::filesystem::current_path().string());
    std::ifstream file("characters.json");

    if (!file.is_open()) {
        std::println(stderr, "Could not find characters.json!");
        hold_input();
        return {};
    }
    nlohmann::json data;
    try {
        file >> data;
    }
    catch (const nlohmann::json::parse_error& e) {
        std::println(stderr, "JSON Parse Error: {}", e.what());
        hold_input();
        return {};
    }

    if (data.contains("characters") && data["characters"].is_array()) {
        std::vector<std::unique_ptr<Character>> list;
        for (const auto& d : data["characters"]) {
            list.push_back(Create::Char::Custom(d));
        }
        return list;
    }
    return {};
}

const std::vector<std::unique_ptr<Character>> SetupSystem::GetCharacterList(bool get_custom) {
    std::vector<std::unique_ptr<Character>> list;
    list.push_back(std::make_unique<Character>(Create::Char::Toji()));
    list.push_back(std::make_unique<Character>(Create::Char::Maki()));
    list.push_back(std::make_unique<CurseUser>(Create::Char::Gojo()));
    list.push_back(std::make_unique<CurseUser>(Create::Char::Sukuna()));
    list.push_back(std::make_unique<CurseUser>(Create::Char::Mahito()));
    return list;
}

Character* SetupSystem::SetupLoop(Battlefield& bf) {
    Character* c{nullptr};
    bool load_json{false};
    while(true){
        std::println("Chosen: [{}]", c ? GetInfo::Name(c->Identity()) : "None");
        std::println("{}", load_json ? "[Custom Characters Enabled]" : "");
        for (const auto& [name, num] : SetupSystem::SetList(bf)){
            std::println("{}x {}", num, name);
        }
        SetupSystem::LogSetupOptions();
        if (!SetupSystem::SetupOptions(bf, c, load_json)) break;
    }
    return c;
}

void SetupSystem::LogSetupOptions() {
    std::println("1 - Add Character         | 11 - Add multiple characters         | 111 - Choose Character\n"
                 "2 - Remove Character      | 22 - Remove last character           | 222 - Choose from battlefield\n"
                 "3 - Clear Battlefield     | 33 - Additional Character Info       | 333 - Load Custom Characters\n"
                 "0 - Start Game");
}

bool SetupSystem::SetupOptions(Battlefield& bf, Character*& c, bool& load_json) {
    switch(get_input<int>()){
        case 1:
            SetupSystem::AddCharacter(bf, c, false, load_json);
            break;
        case 11:
            SetupSystem::AddCharacters(bf, load_json);
            break;
        case 111:
            SetupSystem::AddCharacter(bf, c, true, load_json);
            break;
        case 2:
            SetupSystem::RemoveCharacter(bf, c, load_json);
            break;
        case 22:
            SetupSystem::RemoveLast(bf, c);
            break;
        case 222:
            SetupSystem::RemoveCharacter(bf, c, load_json);
            break;
        case 3:
            SetupSystem::ClearBattlefield(bf, c);
            break;
        case 33:
            SetupSystem::ViewCharacterInfo(bf);
            break;
        case 333:
            load_json = !load_json;
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

void SetupSystem::AddCharacter(Battlefield& bf, Character*& c, bool choosing, bool load_json){
    size_t x{0};
    const auto list = SetupSystem::GetCharacterList(load_json);
    for (const auto& z : list) {
        std::println("{}:{}", ++x, GetInfo::Styalized(z->Identity()));
    }
    x = get_input<size_t>() - 1;
    if (x >= list.size()) {
        return;
    }
    bf.battlefield.push_back(list[x]->Clone());
    if (choosing) {
        c = bf.battlefield.back().get();
    }
}
void SetupSystem::AddCharacters(Battlefield& bf, bool load_json){
    size_t g{0};
    const auto list = SetupSystem::GetCharacterList(load_json);
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