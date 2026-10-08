#pragma once
#include "../CharacterType/CurseUser.hpp"
#include <string>

namespace Stringet {
    const std::string SacrificeStr(const SacrificeType type);
    const std::string OutputCmpStr(const double cur, const double max); // "X/Y"
    const std::string OutputStatusStr(const CurseUserOutput::Status s);
}