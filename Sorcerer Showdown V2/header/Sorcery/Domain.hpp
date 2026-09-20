#pragma once
#include "../Structs.hpp"
#include "../Enums.hpp"

enum class DomainType : std::uint8_t {
    Closed,
    Open
};

enum class SurehitType : std::uint8_t {
    Basic, // neutralizers, even with damage reduction will nullify the domain
    Normal, // neutralizers can withstand the domain but cannot fully nullify it
    Paralyzing // neutralizers must actually neutralize the domains surehit or else they will be paralyzed
};

enum class Refinement : std::uint8_t {
    Brittle,
    Weak,
    Standard,
    Adept,
    Overwhelming,
    Absolue
};

struct Domain final {
    EntityInfo identity;

    double health{1.0};
    double max_health{1.0};
    double durability{1.0};
    double damage{1.0};
    double cost{1.0};
    int range{10};
    bool is_active{false};

    DomainType type{DomainType::Closed};
    Refinement refinement{Refinement::Standard};
    SurehitType surehit_type{SurehitType::Normal};
    globalums::DamageType damage_type{globalums::DamageType::Normal};
};