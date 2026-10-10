#include "../header/Game.hpp"
#include "../header/Logger.hpp"
#include "../header/Battlefield.hpp"
#include "../header/Utilities/Input.hpp"
#include "../header/Systems/SetupSystem.hpp"
#include "../header/Systems/BattlefieldSystem.hpp"
#include "../header/Systems/ResourceHandler.hpp"
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

bool rungameloop(Battlefield& bf, const playerchoices& pc, const Skippy& sp) { // final checklist: Improve logging and finish the AI Implementation
    for(size_t idx = 0; idx < bf.battlefield.size(); ++idx){
        auto& c = bf.battlefield[idx];
        if (c->State().health <= 0.0) continue;
        std::print("({})", idx + 1);
        Log::CharacterPresentation(*c);
        std::println();
        if (pc.user_character == c.get()){
            UserControl::GetPlayerTurn(*c, bf);
        }else {
            AI::Fight(*c, bf);
        }
        std::println();
        if (!sp.skip_turns){
            std::println("end of turn");
            hold_input();
        }
        if (!BattlefieldSystem::CanContinueFight(bf)) break;
    }

    ResourceHandler::TickAll(bf);
    BattlefieldSystem::HandleSpawns(bf);
    BattlefieldSystem::HandleDomainInteraction(bf);
    BattlefieldSystem::HandleDeadPeople(bf);

    if (!sp.skip_all){
        std::println("end of round");
        hold_input();
    }

    return bf.battlefield.size() > 1;
}

bool rungame()  {
    Battlefield bf;
    const playerchoices pc = {SetupSystem::SetupLoop(bf), SetupSystem::GetPlayerSkipType()};
    const Skippy sp{(pc.skip_type == SkipType::Turns || pc.skip_type == SkipType::All), pc.skip_type == SkipType::All};
    while(rungameloop(bf, pc, sp));
    return endgame();
}