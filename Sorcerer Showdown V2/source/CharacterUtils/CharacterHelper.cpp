#include "../../header/CharacterUtils/CharacterHelper.hpp"
#include "../../header/CharacterType/Character.hpp"

#include <format>

std::string Helper::GetName(Character &c, NType type){
    switch(type){
    case NType::Name:   return c.identity.name;
    case NType::Color:  return c.identity.color;
    case NType::Both:   return std::format("{}{}\x1b[0m", c.identity.color, c.identity.name);
    }
    return "";
}