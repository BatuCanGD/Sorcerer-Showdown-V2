#include "../../header/Systems/BattlefieldHandler.hpp"
#include "../../header/Logger.hpp"
#include "../../header/CharacterUtils/CharacterHelper.hpp"
#include "../../header/CharacterType/Character.hpp"
#include "../../header/Utilities/Input.hpp"
#include "../../header/Battlefield.hpp"

#include <print>
#include <memory>
#include <unordered_map>

void BattlefieldHandler::PrintSetupValues(const std::unordered_map<int, std::unique_ptr<Character>>& list) {
    for (auto const& [count, character] : list){
        std::println("{}:{}", count, Helper::GetName(*character));
    }
}


bool BattlefieldHandler::ManageSetupChoices(int x){
    if (x == 0) return false;
    return true;
}


void BattlefieldHandler::SetupBattlefield(battlefield &bf) {
    std::unordered_map<int, std::unique_ptr<Character>> list;
    while(true) {
        PrintSetupValues(list);
        if (!ManageSetupChoices(get_input<int>())) break;
    }
}

void BattlefieldHandler::HandleDeadPeople(battlefield &bf){
    Log::Death(bf);
    std::erase_if(bf.battlefield, [](const auto& s) { 
        return s->Health() <= 0.0;
    });
}