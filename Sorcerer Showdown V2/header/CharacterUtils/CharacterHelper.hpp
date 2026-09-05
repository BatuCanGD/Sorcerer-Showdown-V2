#pragma once
#include "../Enums.hpp"

#include <string>

class Character;

struct Helper {
    static std::string GetName(Character& c, charenums::NameType type = charenums::NameType::Both);
};