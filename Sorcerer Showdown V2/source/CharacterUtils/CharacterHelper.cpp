#include "../../header/CharacterUtils/CharacterHelper.hpp"
#include "../../header/CharacterType/Character.hpp"

#include <format>

std::string Helper::GetName(Character &c, charenums::NameType type){
    switch(type){
    case charenums::NameType::Name:   return c.identity.name;
    case charenums::NameType::Color:  return c.identity.color;
    case charenums::NameType::Both:   return std::format("{}{}\x1b[0m", c.identity.color, c.identity.name);
    }
    return "";
}