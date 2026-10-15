#pragma once
#include "../Info.hpp"
#include <cstdint>

enum class NeutralizerType : std::uint8_t {
    ReducedDamage,
    FullyProtected
};

struct Neutralizer final {
    EntityInfo identity;
    double health{1.0};
    double max_health{1.0};
    double durability{1.0};
    double cost{1.0};
    bool is_active{false};
    NeutralizerType neutralizer_type{NeutralizerType::FullyProtected};
};