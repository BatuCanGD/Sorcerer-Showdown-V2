#include "../header/Game.hpp"
#include "../header/Logger.hpp"
#include "../header/Battlefield.hpp"
#include "../header/Utilities/Input.hpp"
#include "../header/Systems/GameEngine.hpp"

#include <print>

struct playerchoices {
    bool user_playing{false};
    SkipType skip_type{SkipType::None};
};

bool endgame() {
    std::print("Continue Game?\n1 - Continue | 2 - Stop\n=> ");
    return get_input<int>() == 1;
}

bool rungameloop(Battlefield& bf, const playerchoices& pc) {
    for(const auto& c : bf.battlefield){
       Log::CharacterInfo(*c);
       if (pc.skip_type == SkipType::Turns){

       }
    }
    if (pc.skip_type == SkipType::All){

    }
    return false;
}

bool rungame()  {
    Battlefield bf;
    const playerchoices pc = {GameEngine::SetupLoop(bf), GameEngine::GetPlayerSkipType()};

    while(rungameloop(bf, pc));
    return endgame();
}