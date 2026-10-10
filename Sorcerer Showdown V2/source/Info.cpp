#include "../header/Info.hpp"

const std::string GetInfo::Name(const EntityInfo &c){
    return c.color + c.name + (c.color.empty() ? "" : "\x1b[0m");
}
const std::string GetInfo::Styalized(const EntityInfo &c) {
    return GetInfo::Name(c) + '\n' + "﹂" + c.description;
}