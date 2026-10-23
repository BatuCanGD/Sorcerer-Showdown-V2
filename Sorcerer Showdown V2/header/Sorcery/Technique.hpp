#pragma once

#include "../Enums.hpp"
#include "../Info.hpp"

#include <vector>

struct TechAbility final {
    EntityInfo id{};
    double damage{1.0};
    double cost{1.0};
    double output{1.0};
    int chant_amount{0};
    globalums::DamageType damage_type{globalums::DamageType::Normal};
};
struct TechBarrier final {
    bool can_use_barrier{false};
    bool is_active{false};
};

struct Technique final {
    std::vector<TechAbility> abilities;
    TechBarrier barrier;
    EntityInfo identity;
};