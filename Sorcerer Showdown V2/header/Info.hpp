#pragma once
#include <string>

struct EntityInfo final {
    std::string name{"Nobody"};
    std::string color{""};
    std::string description{""};
};

namespace GetInfo {
    const std::string Name(const EntityInfo& c);
    const std::string Styalized(const EntityInfo& c);
}