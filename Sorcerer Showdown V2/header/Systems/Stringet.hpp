#pragma once
#include <string>
#include <cstdint>
enum class SacrificeType : std::uint8_t;

namespace Stringet {
    const std::string SacrificeStr(const SacrificeType type);
    const std::string OutputCmpStr(const double cur, const double max); // "X/Y"
}