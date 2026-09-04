#include "../header/Game.hpp"
#include "../header/CharacterType/Character.hpp"
#include "../header/CharacterType/Sorcerer.hpp"
#include "../header/Utilities/Input.hpp"
#include "../header/Battlefield.hpp"
#include "../header/CharacterUtils/CharacterCreator.hpp"
#include "../header/CombatHelper.hpp"
#include "../header/Logger.hpp"

#include <print>

bool endgame() {
    std::print("Continue Game?\n1 - Continue | 2 - Stop\n=> ");
    return get_input<int>() == 1;
}

bool rungame()  {
    battlefield bf;
    bf.battlefield.push_back(Create::Gojo());
    bf.battlefield.push_back(Create::Mahito());
    auto ats = bf.battlefield[0]->Attack(*bf.battlefield[1].get());
    Log::Attack(ats, *bf.battlefield[0].get(), *bf.battlefield[1].get());
    return endgame();
}