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
struct TechBarrier final {
    bool can_use_barrier{false};
    bool is_active{false};
};

class Technique final {
    friend struct TechniqueEditor;
    friend struct TechniqueSystem;
protected:
    std::vector<TechAbility> abilities;
    TechBarrier barrier;
    EntityInfo identity;
public:
    const EntityInfo& Identity() const noexcept;
    const TechBarrier& Barrier() const noexcept;
    void Barrier(bool set);
    bool HasBarrier() const noexcept;

    void PrintName() const;
    void PrintAbilities() const;
    const TechAbility& GetAbility(size_t idx) const;
    size_t GetAbility() const;
};