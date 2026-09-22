#include "../header/Game.hpp"
#include "../header/Logger.hpp"
#include "../header/Creator.hpp"
#include "../header/Battlefield.hpp"
#include "../header/Utilities/Input.hpp"
#include "../header/Systems/GameEngine.hpp"
#include "../header/CharacterType/CurseUser.hpp"

#include <print>
#include <vector>

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
    }
    return false;
}

bool rungame()  {
    Battlefield bf;
    const playerchoices pc = {GameEngine::PlayerSetupBattlefield(bf), GameEngine::GetPlayerSkipType()};

    while(rungameloop(bf, pc));
    return endgame();
}