#pragma once
#include "../Structs.hpp"

enum class NeutralizerTurnType {
    SelfSustained, // use it once, you are good until it gets destroyed
    UserSustained, // sustained by cursed energy, user has to take care of it every turn
};
enum class NeutralizerType {
    FullyProtected,
    ReducedDamage
};

struct Neutralizer final {
    EntityInfo identity;
    double health{1.0};
    double durability{1.0};
    double cost{1.0};
    NeutralizerTurnType turn_type{NeutralizerTurnType::SelfSustained};
    NeutralizerType neutralizer_type{NeutralizerType::FullyProtected};
};