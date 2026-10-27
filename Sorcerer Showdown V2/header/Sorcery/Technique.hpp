#pragma once
#include "../General/StatusEffects.hpp"
#include "../DamageType.hpp"
#include "../Info.hpp"

#include <vector>
#include <optional>

struct TechAbility final {
    EntityInfo id{};
    std::optional<StatusEffect> given_effect;
    double damage{1.0};
    double cost{1.0};
    double output{1.0};
    int chant_amount{0};
    DamageType damage_type{DamageType::Normal};
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