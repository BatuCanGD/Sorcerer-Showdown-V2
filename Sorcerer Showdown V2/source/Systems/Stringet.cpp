#include "../../header/Systems/Stringet.hpp"

#include <format>
#include <array>

struct StringData final {
    double threshold;
    std::string_view text;
    std::string_view color;
};

const std::string Stringet::HealthStr(const double hp){
    static constexpr std::array<StringData, 9> health_data{
        StringData{25.0,   "VERY LOW",      "\x1b[38;5;8m"},
        StringData{50.0,   "LOW",           "\x1b[38;5;1m"},
        StringData{75.0,   "BELOW AVERAGE", "\x1b[38;5;3m"},
        StringData{100.0,  "AVERAGE",       "\x1b[38;5;7m"},
        StringData{150.0,  "HEALTHY",       "\x1b[38;5;2m"},
        StringData{200.0,  "HIGH",          "\x1b[38;5;10m"},
        StringData{300.0,  "VERY HIGH",     "\x1b[38;5;14m"},
        StringData{500.0,  "EXCEPTIONAL",   "\x1b[38;5;13m"},
        StringData{1000.0, "OVERWHELMING",  "\x1b[38;5;5m"}
    };

    for (const auto& data : health_data) {
        if (hp < data.threshold) {
            return std::format("{}{}\x1b[0m", data.color, data.text);
        }
    }
    return "\x1b[48;5;5m\x1b[1mABSOLUTE\x1b[0m";
}

const std::string Stringet::DurabilityStr(const double dr){
    static constexpr std::array<StringData, 9> durability_data{
        StringData{25.0,   "VERY FRAGILE", "\x1b[38;5;8m"},
        StringData{50.0,   "FRAGILE",      "\x1b[38;5;1m"},
        StringData{75.0,   "BRITTLE",      "\x1b[38;5;3m"},
        StringData{100.0,  "AVERAGE",      "\x1b[38;5;7m"},
        StringData{150.0,  "DURABLE",      "\x1b[38;5;2m"},
        StringData{200.0,  "TOUGH",        "\x1b[38;5;10m"},
        StringData{300.0,  "VERY TOUGH",   "\x1b[38;5;14m"},
        StringData{500.0,  "EXCEPTIONAL",  "\x1b[38;5;13m"},
        StringData{1000.0, "SUPREME",      "\x1b[38;5;5m"}
    };

    for (const auto& data : durability_data) {
        if (dr < data.threshold) {
            return std::format("{}{}\x1b[0m", data.color, data.text);
        }
    }
    return "\x1b[48;5;5m\x1b[1mABSOLUTE\x1b[0m";
}

const std::string Stringet::StrengthStr(const double str){
    static constexpr std::array<StringData, 9> strength_data{
        StringData{25.0,   "VERY WEAK",     "\x1b[38;5;8m"},
        StringData{50.0,   "WEAK",          "\x1b[38;5;1m"},
        StringData{75.0,   "BELOW AVERAGE", "\x1b[38;5;3m"},
        StringData{100.0,  "AVERAGE",       "\x1b[38;5;7m"},
        StringData{150.0,  "SOLID",         "\x1b[38;5;2m"},
        StringData{200.0,  "STRONG",        "\x1b[38;5;10m"},
        StringData{300.0,  "VERY STRONG",   "\x1b[38;5;14m"},
        StringData{500.0,  "EXCEPTIONAL",   "\x1b[38;5;13m"},
        StringData{1000.0, "SUPREME",       "\x1b[38;5;5m"}
    };

    for (const auto& data : strength_data) {
        if (str < data.threshold) {
            return std::format("{}{}\x1b[0m", data.color, data.text);
        }
    }
    return "\x1b[48;5;5m\x1b[1mABSOLUTE\x1b[0m";
}

struct EnumStringData final {
    std::string_view text;
    std::string_view color;
};

const std::string Stringet::EfficiencyStr(const CursedEnergySystem::Efficiency& efficiency){
    static constexpr std::array<EnumStringData, 8> efficiency_data{
        EnumStringData{"Wasteful", "\x1b[38;5;124m"},
        EnumStringData{"Rough",    "\x1b[38;5;202m"},
        EnumStringData{"Unstable", "\x1b[38;5;221m"},
        EnumStringData{"Stable",   "\x1b[38;5;118m"},
        EnumStringData{"Expert",   "\x1b[38;5;10m"},
        EnumStringData{"Absolute", "\x1b[38;5;50m"},
        EnumStringData{"Ultimate", "\x1b[38;5;87m"},
        EnumStringData{"Extreme",  "\x1b[38;5;27m"}
    };

    const auto& data = efficiency_data[static_cast<std::size_t>(efficiency)];
    return std::format("{}{}\x1b[0m", data.color, data.text);
}

const std::string Stringet::OutputStr(const double op) {
    static constexpr std::array<StringData, 9> output_data{
        StringData{25.0,   "EXTREMELY LOW", "\x1b[38;5;8m"},
        StringData{50.0,   "VERY LOW",      "\x1b[38;5;1m"},
        StringData{75.0,   "LOW",           "\x1b[38;5;3m"},
        StringData{100.0,  "BELOW AVERAGE", "\x1b[38;5;7m"},
        StringData{150.0,  "AVERAGE",       "\x1b[38;5;2m"},
        StringData{200.0,  "HIGH",          "\x1b[38;5;10m"},
        StringData{300.0,  "VERY HIGH",     "\x1b[38;5;14m"},
        StringData{500.0,  "EXCEPTIONAL",   "\x1b[38;5;13m"},
        StringData{1000.0, "SUPREME",       "\x1b[38;5;5m"}
    };

    for (const auto& data : output_data) {
        if (op < data.threshold) {
            return std::format("{}{}\x1b[0m", data.color, data.text);
        }
    }
    return "\x1b[48;5;5m\x1b[1mABSOLUTE\x1b[0m";
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