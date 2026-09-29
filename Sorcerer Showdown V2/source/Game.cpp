#include "../header/Game.hpp"
#include "../header/Logger.hpp"
#include "../header/Battlefield.hpp"
#include "../header/Utilities/Input.hpp"
#include "../header/Systems/SetupSystem.hpp"
#include "../header/Systems/PlayerSystem.hpp"

#include <print>

struct playerchoices final {
    Character* user_character{nullptr};
    SkipType skip_type{SkipType::None};
};

struct Skippy final {
    bool skip_turns{false};
    bool skip_all{false};
};

bool endgame() {
    std::print("Continue Game?\n1 - Continue | 2 - Stop\n=> ");
    return get_input<int>() == 1;
}

bool rungameloop(Battlefield& bf, const playerchoices& pc, const Skippy& sp) {
    for(const auto& c : bf.battlefield){
        Log::CharacterInfo(*c);
        if (pc.user_character == c.get()){
            UserControl::GetPlayerTurn(*c, bf);
        }else {
            AI::Fight(*c, bf);
        }
        if (sp.skip_turns){
            hold_input();
        }
    }


    if (sp.skip_all){
        hold_input();
    }
    if (bf.battlefield.size() <= 1){
        return false;
    }
    return true;
}

bool rungame()  {
    Battlefield bf;
    const playerchoices pc = {SetupSystem::SetupLoop(bf), SetupSystem::GetPlayerSkipType()};
    const Skippy sp{(pc.skip_type == SkipType::Turns || pc.skip_type == SkipType::All), pc.skip_type == SkipType::All};
    while(rungameloop(bf, pc, sp));
    return endgame();
}