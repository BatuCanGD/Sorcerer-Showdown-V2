#pragma once

#include "../Enums.hpp"
#include "../Structs.hpp"

#include <vector>

struct TechAbility final {
    double damage{1.0};
    double cost{1.0};
    double output{1.0};
    EntityInfo id{};
    globalums::DamageType damage_type{globalums::DamageType::Normal};
};

class Technique final {
    friend struct TechniqueEditor;
    friend struct TechniqueSystem;
protected:
    std::vector<TechAbility> abilities;
    EntityInfo identity;
public:
    void PrintName() const;
    void PrintAbilities() const;
    TechAbility GetAbility(size_t idx);
    size_t GetAbility() const;
};