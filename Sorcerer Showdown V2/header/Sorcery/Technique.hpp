#pragma once

#include "../Enums.hpp"

#include <string>
#include <vector>

struct TechIdentity final {
    std::string name{""};
    std::string color{""};
    std::string description{""};
};

struct TechAbility final {
    double damage{1.0};
    double cost{1.0};
    double output{1.0};
    TechIdentity identity{};
    globalums::DamageType damage_type{globalums::DamageType::Normal};
};

class Technique final {
    friend struct TechniqueEditor;
    friend struct TechniqueSystem;
protected:
    std::vector<TechAbility> abilities;
    TechIdentity identity;
public:
    void PrintName() const;
    void PrintAbilities() const;
    TechAbility GetAbility(size_t idx);
    size_t GetAbility() const;
};