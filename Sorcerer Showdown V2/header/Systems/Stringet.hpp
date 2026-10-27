#pragma once
#include "../Sorcery/BindingVow.hpp"
#include "../SorceryStruct.hpp"
#include <string>

namespace Stringet {
    const std::string SacrificeStr(const SacrificeType type);
    const std::string OutputCmpStr(const double cur, const double max); // "X/Y"
    const std::string OutputStatusStr(const Sorcery::OutputSystem::Status s);
}