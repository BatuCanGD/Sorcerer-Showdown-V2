#pragma once
#include "../Structs.hpp"
#include "../Enums.hpp"

enum class SurehitType {
    Basic, // neutralizers, even with damage reduction will nullify the domain
    Normal, // neutralizers can withstand the domain but cannot fully nullify it
    Paralyzing // neutralizers must actually neutralize the domains surehit or else they will be paralyzed
};  

struct Domain final {
    EntityInfo identity;
    double health{1.0};
    double damage{1.0};
    double cost{1.0};
    unsigned int range{10};
    SurehitType surehit_type{SurehitType::Normal};
    globalums::DamageType damage_type{globalums::DamageType::Normal};
};