#include "../header/Logger.hpp"
#include "../header/CombatHelper.hpp"
#include "../header/CharacterUtils/CharacterHelper.hpp"

#include <print>
#include <string>
#include <format>

void Log::Attack(AttackStruct ats, Character& c1, Character& c2) {
    std::string word{};
    const std::string info = std::format("{} took {:.1f} damage from {}!", Helper::GetName(c1), ats.damage, Helper::GetName(c2));
    
    if (ats.is_critical){
        word.append("\x1b[38;5;124m[CRITICAL]\x1b[0m");
    }
    if (ats.is_blackflash){
        word.append("\x1b[38;5;9m[BLACKFLASH]\x1b[0m");
    }
    word.append(info);

    std::println("{}", word);
}