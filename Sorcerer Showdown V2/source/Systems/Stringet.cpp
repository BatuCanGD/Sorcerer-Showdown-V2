#include "../../header/Systems/Stringet.hpp"

#include <format>

const std::string Stringet::HealthStr(const double hp){
    std::string clr{"\x1b[38;5;9m"};
    std::string s{"Unknown"};

    if (hp < 25.0){
        s = "VERY LOW";
        clr = "\x1b[38;5;8m";
    }else if (hp < 50.0){
        s = "LOW";
        clr = "\x1b[38;5;1m";
    }else if (hp < 75.0){
        s = "BELOW AVERAGE";
        clr = "\x1b[38;5;3m";
    }else if (hp < 100.0){
        s = "AVERAGE";
        clr = "\x1b[38;5;7m";
    }else if (hp < 150.0){
        s = "HEALTHY";
        clr = "\x1b[38;5;2m";
    }else if (hp < 200.0){
        s = "HIGH";
        clr = "\x1b[38;5;10m";
    }else if (hp < 300.0){
        s = "VERY HIGH";
        clr = "\x1b[38;5;14m";
    }else if (hp < 500.0){
        s = "EXCEPTIONAL";
        clr = "\x1b[38;5;13m";
    }else if (hp < 1000.0){
        s = "OVERWHELMING";
        clr = "\x1b[38;5;5m";
    }else {
        s = "ABSOLUTE";
        clr = "\x1b[48;5;5m\x1b[1m";
    }
    return clr + s + "\x1b[0m";
}

const std::string Stringet::DurabilityStr(const double dr){
    std::string clr{"\x1b[38;5;9m"};
    std::string s{"Unknown"};

    if (dr < 25.0){
        s = "VERY FRAGILE";
        clr = "\x1b[38;5;8m";
    }else if (dr < 50.0){
        s = "FRAGILE";
        clr = "\x1b[38;5;1m";
    }else if (dr < 75.0){
        s = "BRITTLE";
        clr = "\x1b[38;5;3m";
    }else if (dr < 100.0){
        s = "AVERAGE";
        clr = "\x1b[38;5;7m";
    }else if (dr < 150.0){
        s = "DURABLE";
        clr = "\x1b[38;5;2m";
    }else if (dr < 200.0){
        s = "TOUGH";
        clr = "\x1b[38;5;10m";
    }else if (dr < 300.0){
        s = "VERY TOUGH";
        clr = "\x1b[38;5;14m";
    }else if (dr < 500.0){
        s = "EXCEPTIONAL";
        clr = "\x1b[38;5;13m";
    }else if (dr < 1000.0){
        s = "SUPREME";
        clr = "\x1b[38;5;5m";
    }else {
        s = "ABSOLUTE";
        clr = "\x1b[48;5;5m\x1b[1m";
    }
    return clr + s + "\x1b[0m";
}

const std::string Stringet::StrengthStr(const double str){
    std::string clr{"\x1b[38;5;9m"};
    std::string s{"Unknown"};

    if (str < 25.0){
        s = "VERY WEAK";
        clr = "\x1b[38;5;8m";
    }else if (str < 50.0){
        s = "WEAK";
        clr = "\x1b[38;5;1m";
    }else if (str < 75.0){
        s = "BELOW AVERAGE";
        clr = "\x1b[38;5;3m";
    }else if (str < 100.0){
        s = "AVERAGE";
        clr = "\x1b[38;5;7m";
    }else if (str < 150.0){
        s = "SOLID";
        clr = "\x1b[38;5;2m";
    }else if (str < 200.0){
        s = "STRONG";
        clr = "\x1b[38;5;10m";
    }else if (str < 300.0){
        s = "VERY STRONG";
        clr = "\x1b[38;5;14m";
    }else if (str < 500.0){
        s = "EXCEPTIONAL";
        clr = "\x1b[38;5;13m";
    }else if (str < 1000.0){
        s = "SUPREME";
        clr = "\x1b[38;5;5m";
    }else {
        s = "ABSOLUTE";
        clr = "\x1b[48;5;5m\x1b[1m";
    }
    return clr + s + "\x1b[0m";
}

const std::string Stringet::EfficiencyStr(const CursedEnergySystem::Efficiency& efficiency){
    std::string clr{"\x1b[38;5;9m"};
    std::string s{"Unknown"};

    switch(efficiency){
        case CursedEnergySystem::Efficiency::Wasteful:  
            clr = "\x1b[38;5;124m";
            s = "Wasteful";
            break;
        case CursedEnergySystem::Efficiency::Rough:     
            clr = "\x1b[38;5;202m";
            s = "Rough";
            break;
        case CursedEnergySystem::Efficiency::Unstable:  
            clr = "\x1b[38;5;221m";
            s = "Unstable";
            break;
        case CursedEnergySystem::Efficiency::Stable:    
            clr = "\x1b[38;5;118m";
            s = "Stable";
            break;
        case CursedEnergySystem::Efficiency::Expert:    
            clr = "\x1b[38;5;10m";
            s = "Expert";
            break;
        case CursedEnergySystem::Efficiency::Absolute:  
            clr = "\x1b[38;5;50m";
            s = "Absolute";
            break;
        case CursedEnergySystem::Efficiency::Ultimate:  
            clr = "\x1b[38;5;87m";
            s = "Ultimate";
            break;
        case CursedEnergySystem::Efficiency::Extreme:   
            clr = "\x1b[38;5;27m";
            s = "Extreme";
            break;
    }
    return clr + s + "\x1b[0m";
}

const std::string Stringet::OutputStr(const double op) {
    std::string clr{"\x1b[38;5;9m"};
    std::string s{"Unknown"};

    if (op < 25.0){
        s = "EXTREMELY LOW";
        clr = "\x1b[38;5;8m";
    }else if (op < 50.0){
        s = "VERY LOW";
        clr = "\x1b[38;5;1m";
    }else if (op < 75.0){
        s = "LOW";
        clr = "\x1b[38;5;3m";
    }else if (op < 100.0){
        s = "BELOW AVERAGE";
        clr = "\x1b[38;5;7m";
    }else if (op < 150.0){
        s = "AVERAGE";
        clr = "\x1b[38;5;2m";
    }else if (op < 200.0){
        s = "HIGH";
        clr = "\x1b[38;5;10m";
    }else if (op < 300.0){
        s = "VERY HIGH";
        clr = "\x1b[38;5;14m";
    }else if (op < 500.0){
        s = "EXCEPTIONAL";
        clr = "\x1b[38;5;13m";
    }else if (op < 1000.0){
        s = "SUPREME";
        clr = "\x1b[38;5;5m";
    }else {
        s = "ABSOLUTE";
        clr = "\x1b[48;5;5m\x1b[1m";
    }
    return clr + s + "\x1b[0m";
}


const std::string Stringet::OutputCmpStr(const double cur, const double max){
    std::string clr{"\x1b[38;5;9m"};
    if (cur > max * 0.75){
        clr = "\x1b[38;5;82m";
    }else if (cur > max * 0.50){
        clr = "\x1b[38;5;190m";
    }else if (cur > max * 0.25){
        clr = "\x1b[38;5;208m";
    }
    return std::format("{}{}/{}\x1b[0m", clr, cur, max);
}