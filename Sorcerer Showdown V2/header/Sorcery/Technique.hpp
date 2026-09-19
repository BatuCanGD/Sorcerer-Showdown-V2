#pragma once

#include "../Enums.hpp"
#include "../Structs.hpp"

#include <vector>

struct TechAbility final {
    EntityInfo id{};
    double damage{1.0};
    double cost{1.0};
    double output{1.0};
    globalums::DamageType damage_type{globalums::DamageType::Normal};
};
struct TechBarrier final {
    bool can_use_barrier{false};
    bool is_active{false};
};

class Technique final {
protected:
    std::vector<TechAbility> abilities;
    TechBarrier barrier;
    EntityInfo identity;
public:
    [[nodiscard]] const EntityInfo& Identity() const noexcept;
    [[nodiscard]] const TechBarrier& Barrier() const noexcept;
    [[nodiscard]] const std::vector<TechAbility>& Abilities() const noexcept;

    EntityInfo& Identity() noexcept;
    TechBarrier& Barrier() noexcept;
    std::vector<TechAbility>& Abilities() noexcept;

    const std::string Name() const noexcept;


    void Barrier(bool set);
    bool HasBarrier() const noexcept;

    const TechAbility& GetAbility(size_t idx) const;
    size_t GetAbility() const;
};