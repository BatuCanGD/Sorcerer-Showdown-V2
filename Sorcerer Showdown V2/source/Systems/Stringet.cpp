#include "../../header/Systems/Stringet.hpp"
#include "../../header/Sorcery/BindingVow.hpp"
#include <format>

const std::string Stringet::OutputCmpStr(const double cur, const double max){
    std::string clr{"\x1b[38;5;9m"};
    if (cur > max * 0.75){
        clr = "\x1b[38;5;82m";
    }else if (cur > max * 0.50){
        clr = "\x1b[38;5;190m";
    }else if (cur > max * 0.25){
        clr = "\x1b[38;5;208m";
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