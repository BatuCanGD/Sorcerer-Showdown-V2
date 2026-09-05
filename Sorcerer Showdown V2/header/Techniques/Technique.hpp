#pragma once

#include "../Enums.hpp"

#include <string>
#include <vector>

struct TechIdentity{
    std::string name{""};
    std::string color{""};
    std::string description{""};
};

struct TechAbility final {
    double damage{1.0};
    std::string name;
    std::string color;
    globalums::DamageType damage_type = globalums::DamageType::Normal;
};


class Technique final {
    friend struct Editor;
protected:
    std::vector<TechAbility> abilities;
    TechIdentity identity;
};