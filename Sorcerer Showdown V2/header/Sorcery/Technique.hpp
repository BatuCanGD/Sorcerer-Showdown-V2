#pragma once
#include "../General/StatusEffects.hpp"
#include "../DamageType.hpp"
#include "../Info.hpp"

#include <vector>
#include <optional>

enum class AttackType : std::uint8_t {
    Singular,
    AreaOfEffect
};
enum class RequiredStat : std::uint8_t {
    None,
    HealthLow,
    HealthMid,
    HealthHigh
};

struct TechRequirement final {
    RequiredStat stat_req{RequiredStat::None};
    std::uint8_t chant_req{0};
};

struct TechAbility final {
    EntityInfo id{};
    std::optional<StatusEffect> given_effect;
    std::optional<TechRequirement> requirement;
    double damage{1.0};
    double cost{1.0};
    double output{1.0};
    int chant_amount{0};
    AttackType attack_type{AttackType::Singular};
    DamageType damage_type{DamageType::Normal};
};
struct TechBarrier final {
    enum class Status : std::uint8_t { Unusable, Inactive, Active};
    Status status{Status::Unusable};
};

struct Technique final {
    std::vector<TechAbility> abilities;
    TechBarrier barrier;
    EntityInfo identity;
};