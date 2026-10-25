#include "../../header/Systems/SetupSystem.hpp"
#include "../../header/Json.hpp"
#include "../../header/CharacterType/CurseUser.hpp"
#include "../../header/Battlefield.hpp"
#include "../../header/Utilities/Input.hpp"
#include "../../header/Creator.hpp"
#include "../../header/Logger.hpp"
#include <map>

SkipType SetupSystem::GetPlayerSkipType() noexcept {
    Log::Line("1 - Skip Everything\n2 - Skip to end of round\n3 - Dont skip");
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

const std::vector<std::unique_ptr<Character>> SetupSystem::GetCharacterList(bool get_custom) {
    std::vector<std::unique_ptr<Character>> list;
    list.push_back(std::make_unique<CurseUser>(Create::Char::Gojo()));
    list.push_back(std::make_unique<CurseUser>(Create::Char::Sukuna()));
    list.push_back(std::make_unique<CurseUser>(Create::Char::Jogo()));
    list.push_back(std::make_unique<CurseUser>(Create::Char::Hanami()));
    list.push_back(std::make_unique<CurseUser>(Create::Char::Mahito()));
    list.push_back(std::make_unique<Character>(Create::Char::Toji()));
    list.push_back(std::make_unique<Character>(Create::Char::Maki()));
    if (get_custom){
        Json::LoadCharacters(list);
    }
    return list;
}

Character* SetupSystem::SetupLoop(Battlefield& bf) {
    Character* c{nullptr};
    bool load_json{false};
    while(true){
        Log::Line("Chosen: [{}]", c ? GetInfo::Name(c->Identity()) : "None");
        Log::Line("{}", load_json ? "[Custom Characters Enabled]" : "");
        for (const auto& [name, num] : SetupSystem::SetList(bf)){
            Log::Line("{}x {}", num, name);
        }
        const auto opts = SetupSystem::GetSetupOptions(c);
        if (!SetupSystem::SetupOptions(opts, bf, c, load_json)) break;
    }
    Log::Clear();
    return c;
}

std::vector<SetupSystem::SetupOpt> SetupSystem::GetSetupOptions(const Character* c) {
    std::vector<SetupSystem::SetupOpt> opt;
    auto add_opt = [&opt](std::string_view v, SetupSystem::SetupOpt op){
        Log::Line("{}:{}", opt.size(), v);
        opt.push_back(op);
    };
    add_opt("Start", SetupSystem::SetupOpt::Start);
    add_opt("Add Character", SetupSystem::SetupOpt::AddChar);
    add_opt("Add Multiple", SetupSystem::SetupOpt::AddMult);
    add_opt("Remove Specific Character", SetupSystem::SetupOpt::RemoveSpecific);
    add_opt("Remove Last Character", SetupSystem::SetupOpt::RemoveLast);
    if (c) add_opt("Remove Chosen Character", SetupSystem::SetupOpt::RemoveChosen);
    add_opt("Choose and Add Character", SetupSystem::SetupOpt::Choose);
    add_opt("Choose from Battlefield", SetupSystem::SetupOpt::ChooseBf);
    add_opt("Clear Battlefield", SetupSystem::SetupOpt::Clear);
    add_opt("View Character Information", SetupSystem::SetupOpt::CharInfo);
    add_opt("Load JSON", SetupSystem::SetupOpt::Json);
    return opt;
}

bool SetupSystem::SetupOptions(const std::vector<SetupOpt>& opts, Battlefield& bf, Character*& c, bool& load_json) {
    const size_t op = get_input<size_t>();
    if (op >= opts.size()) {
        Log::Error("Invalid Choice");
        return true;
    }
    switch(opts[op]){
        case SetupSystem::SetupOpt::Start:
            if (bf.battlefield.size() < 2){
                Log::Line("Not enough fighters");
                break;
            }
            return false;
        case SetupSystem::SetupOpt::AddChar:
            SetupSystem::AddCharacter(bf, c, false, load_json);
            break;
        case SetupSystem::SetupOpt::AddMult:
            SetupSystem::AddCharacters(bf, load_json);
            break;
        case SetupSystem::SetupOpt::Choose:
            SetupSystem::AddCharacter(bf, c, true, load_json);
            break;
        case SetupSystem::SetupOpt::RemoveSpecific:
            SetupSystem::RemoveCharacter(bf, c, false);
            break;
        case SetupSystem::SetupOpt::RemoveLast:
            SetupSystem::RemoveLast(bf, c);
            break;
        case SetupSystem::SetupOpt::ChooseBf:
            SetupSystem::RemoveCharacter(bf, c, true);
            break;
        case SetupSystem::SetupOpt::Clear:
            SetupSystem::ClearBattlefield(bf, c);
            break;
        case SetupSystem::SetupOpt::CharInfo:
            SetupSystem::ViewCharacterInfo(bf);
            break;
        case SetupSystem::SetupOpt::Json:
            load_json = !load_json;
            break;
        case SetupSystem::SetupOpt::RemoveChosen:
            c = nullptr;
            break;
        default:
            Log::Error("Invalid Choice");
            break;
    }
    return true;
}

void SetupSystem::AddCharacter(Battlefield& bf, Character*& c, bool choosing, bool load_json){
    size_t x{0};
    const auto list = SetupSystem::GetCharacterList(load_json);
    for (const auto& z : list) {
        Log::Line("{}:{}", ++x, GetInfo::Styalized(z->Identity()));
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
        Log::Line("{}:{}", ++g, GetInfo::Name(z->Identity()));
    }
    Log::Line("Which character would you like to add");
    g = get_input<size_t>() - 1;
    if (g >= list.size()){
        return;
    }
    Log::Line("How many?\n=> ");
    int x = get_input<int>();
    for (int i = 0; i < x; i++){
        bf.battlefield.push_back(list[g]->Clone());
    }
}

void SetupSystem::RemoveCharacter(Battlefield& bf, Character*& c, bool p_choose){
    if (bf.battlefield.empty()) return;
    size_t x{0};
    for (const auto& b : bf.battlefield){
        Log::Line("{}:{} {}", ++x, GetInfo::Name(b->Identity()), c == &*b ? "(Chosen)" : "");
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
        Log::Line("{}:{}", ++x, GetInfo::Name(c->Identity()));
    }
    Log::Line("Who's info would you like to view");
    x = get_input<size_t>();
    if (x == 0 || x > bf.battlefield.size()){
        return;
    }
    Log::CharacterPresentation(*bf.battlefield[--x]);
}