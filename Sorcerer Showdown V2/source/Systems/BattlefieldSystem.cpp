#include "../../header/Systems/BattlefieldSystem.hpp"
#include "../../header/Logger.hpp"
#include "../../header/Battlefield.hpp"

void BattlefieldSystem::HandleDeadPeople(Battlefield &bf){
    Log::Death(bf);
    std::erase_if(bf.battlefield, [](const auto& s) { 
        return s->State().health <= 0.0;
    });
}

void BattlefieldSystem::HandleSpawns(Battlefield &bf) {
    for (auto& c : bf.spawn_next){
        bf.battlefield.push_back(std::move(c));
    }
}