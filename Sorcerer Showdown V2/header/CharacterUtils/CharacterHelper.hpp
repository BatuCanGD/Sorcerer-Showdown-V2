#pragma once

#include <string>

class Character;

enum class NType {
    Name,
    Color,
    Both
};

struct Helper {
    static std::string GetName(Character& c, NType type = NType::Both);
};