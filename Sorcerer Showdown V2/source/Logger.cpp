#include "../header/Logger.hpp"
#include "../header/Battlefield.hpp"
#include "../header/Systems/Fighting/CombatSystem.hpp"

#include <print>
#include <string>
#include <vector>
#include <format>

void Log::Attack(AttackStruct ats, Character& c1, Character& c2) {
    std::string word{};
    const std::string info = std::format("{} took {:.1f} damage from {}!", c1.Name(), ats.damage, c2.Name());
    
    if (ats.is_critical){
        word.append("\x1b[38;5;124m[CRITICAL]\x1b[0m");
    }
    if (ats.is_blackflash){
        word.append("\x1b[38;5;9m[BLACKFLASH]\x1b[0m");
    }
    word.append(info);

    std::println("{}", word);
}

void Log::Death(battlefield& bf){
    std::vector<std::string> death_messages;
    for (const auto& c : bf.battlefield){
        const double hp = c->Health();
        if (hp > 0.0) continue;
        std::string msg{""}, severity{"DEATH"}, color{"\x1b[38;5;237m"};
        
        if (hp <= -500.0){
            severity = "EXTREME";
            color = "\x1b[38;5;124m";
        }else if (hp <= -250.0){
            severity = "BRUTAL";
            color = "\x1b[38;5;160m";
        }else if (hp <= -100.0){
            severity = "OVERKILL";
            color = "\x1b[38;5;9m";
        }

        msg.append(std::format("[{0}{1}\x1b[0m] ({0}{2}\x1b[0m)", color, severity, hp));
        msg.append(std::format(" {} has been defeated\n", c->Name()));
        death_messages.push_back(msg);
    }
    if (death_messages.empty()){
        std::println("No characters have been defeated this turn");
        return;
    }
    for (const auto& m : death_messages) {
        std::println("{}", m);
    }
}