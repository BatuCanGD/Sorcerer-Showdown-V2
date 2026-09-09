#pragma once
#include <string>

struct EntityId final {
    std::string name{""};
    std::string color{""};
};

struct EntityInfo final {
    std::string name{""};
    std::string color{""};
    std::string description{""};
};