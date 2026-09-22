#include "../header/Game.hpp"
#include "../header/Logger.hpp"
#include "../header/Battlefield.hpp"
#include "../header/Utilities/Input.hpp"
#include "../header/Systems/SetupSystem.hpp"

#include <print>

struct playerchoices {
    Character* user_character{nullptr};
    SkipType skip_type{SkipType::None};
};

bool endgame() {
    std::print("Continue Game?\n1 - Continue | 2 - Stop\n=> ");
    return get_input<int>() == 1;
}

bool rungameloop(Battlefield& bf, const playerchoices& pc) {
    for(const auto& c : bf.battlefield){
        Log::CharacterInfo(*c);
        if (pc.user_character == c.get()){

        }else {

        }
        if (pc.skip_type == SkipType::Turns){
            hold_input();
        }
    }
    if (pc.skip_type == SkipType::All){
        hold_input();
    }
    return false;
}

bool rungame()  {
    Battlefield bf;
    const playerchoices pc = {SetupSystem::SetupLoop(bf), SetupSystem::GetPlayerSkipType()};
    while(rungameloop(bf, pc));
    return endgame();
}