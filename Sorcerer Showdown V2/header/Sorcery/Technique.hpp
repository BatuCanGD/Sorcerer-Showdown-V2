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

enum class ChantReq : std::uint8_t {
    Zero,
    One,
    Two,
    Three,
    Four,
    Five
};
enum class StatReq : std::uint8_t {
    None,
    HealthLow,
    HealthMid,
    HealthHigh,
    CursedEnergyLow,
    CursedEnergyMid,
    CursedEnergyHigh
};

struct TechRequirement final {
    StatReq stat_req{StatReq::None};
    ChantReq chant_req{ChantReq::Zero};
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
    bool can_use_barrier{false};
    bool is_active{false};
};

struct Technique final {
    std::vector<TechAbility> abilities;
    TechBarrier barrier;
    EntityInfo identity;
};