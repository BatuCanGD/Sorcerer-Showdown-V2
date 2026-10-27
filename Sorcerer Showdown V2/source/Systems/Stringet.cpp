#include "../../header/Systems/Stringet.hpp"
#include "../../header/Sorcery/BindingVow.hpp"
#include <format>

const std::string Stringet::OutputCmpStr(const double cur, const double max){
    std::string clr{"\x1b[38;5;75m"};
    if (cur > max * 0.75){
        clr = "\x1b[38;5;9m";
    }else if (cur > max * 0.50){
        clr = "\x1b[38;5;215m";
    }else if (cur > max * 0.25){
        clr = "\x1b[38;5;156m";

    }
    return std::format("{}{:.1f}/{:.1f}\x1b[0m", clr, cur, max);
}

const std::string Stringet::SacrificeStr(const SacrificeType type){
    switch (type) {
        case SacrificeType::Health:
            return "Health";
        case SacrificeType::CursedEnergy:
            return "Cursed Energy";
        case SacrificeType::OutputPotential:
            return "Output";
        default:
            return "Unknown";
    }
}

const std::string Stringet::OutputStatusStr(const Sorcery::OutputSystem::Status s) {
    std::string n{}, c{};
    switch(s){
        case Sorcery::OutputSystem::Status::BurntOut: 
            c = "\x1b[31m";
            n = "Burnt Out";
            break;
        case Sorcery::OutputSystem::Status::Regular:
            c = "\x1b[38;5;208m";
            n = "Regular Output";
            break;
        case Sorcery::OutputSystem::Status::Boosted:  
            c = "\x1b[36";
            n = "Increased Output";
            break;
        case Sorcery::OutputSystem::Status::Max:
            c = "\x1b[34";
            n = "120%";
            break;
    }
    return c + n + "\x1b[0m";
}