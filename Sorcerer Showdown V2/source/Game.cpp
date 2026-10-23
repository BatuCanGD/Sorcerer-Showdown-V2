#include "../header/Game.hpp"
#include "../header/Logger.hpp"
#include "../header/Battlefield.hpp"
#include "../header/Utilities/Input.hpp"
#include "../header/Systems/SetupSystem.hpp"
#include "../header/Systems/BattlefieldSystem.hpp"
#include "../header/Systems/ResourceHandler.hpp"
#include "../header/Systems/PlayerSystem.hpp"

struct playerchoices final {
    Character* user_character{nullptr};
    SkipType skip_type{SkipType::None};
};

struct Skippy final {
    bool skip_turns{false};
    bool skip_all{false};
};

bool announce_end(Battlefield& bf, const playerchoices& pc){
    auto& b = bf.battlefield;
    if (b.size() < 2){
        if (b.size() != 0){
            auto& chr = b[0];
            Log::Line("{} is the last fighter standing!", GetInfo::Name(chr->Identity()));
            if (pc.user_character == &*chr){
                Log::Line("You Win!");
            }else{
                Log::Line("You Lose!");
            }
        }else{
            Log::Line("No one is left standing alive!");
        }
        return false;
    }
    return true;
}

bool endgame() {
    Log::Line("Continue Game?\n1 - Continue | 2 - End\n=> ");
    return get_input<int>() == 1;
}

bool rungameloop(Battlefield& bf, const playerchoices& pc, const Skippy& sp) { // final checklist: Improve logging and finish the AI Implementation
    for(size_t idx = 0; idx < bf.battlefield.size(); ++idx){
        auto& c = bf.battlefield[idx];
        if (c->State().health <= 0.0) continue;
        Log::Line("({})", idx + 1);
        Log::CharacterPresentation(*c);
        Log::Line();
        if (pc.user_character == c.get()){
            UserControl::GetPlayerTurn(*c, bf);
            hold_input();
        }else {
            AI::Fight(*c, bf);
            if (!sp.skip_turns){
                Log::Line("end of turn");
                hold_input();
            }
        }
        Log::Line();
        if (!BattlefieldSystem::CanContinueFight(bf)) break;
    }

    ResourceHandler::TickAll(bf);
    BattlefieldSystem::HandleSpawns(bf);
    BattlefieldSystem::HandleDomainInteraction(bf);
    BattlefieldSystem::HandleDeadShikigami(bf);
    BattlefieldSystem::HandleDeadPeople(bf);

    if (!sp.skip_all){
        Log::Line("End of round");
        hold_input();
    }
    Log::Clear();
    return announce_end(bf, pc);
}

bool rungame()  {
    Battlefield bf;
    const playerchoices pc = {SetupSystem::SetupLoop(bf), SetupSystem::GetPlayerSkipType()};
    const Skippy sp{(pc.skip_type == SkipType::Turns || pc.skip_type == SkipType::All), pc.skip_type == SkipType::All};
    while(rungameloop(bf, pc, sp));
    return endgame();
}