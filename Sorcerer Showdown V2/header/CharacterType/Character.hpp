#pragma once

#include "../Stuff/StatusEffects.hpp"
#include "../Stuff/CursedTool.hpp"
#include "../AI.hpp"
#include "../Enums.hpp"
#include "../Structs.hpp"

#include <string>
#include <optional>
#include <vector>

class CurseUser;
struct AttackStruct;
struct DamageStruct;

struct BattleIQ final {
    TargetingType targeting_type{TargetingType::Mixed};
    FightingStyle fighting_style{FightingStyle::Mixed};
    ResourceUsage resource_usage{ResourceUsage::Mixed};
};

struct CharState final {
    std::vector<StatusEffect> status_effects;
    double health{1.0};
    double max_health{1.0};
    double durability{1.0};
    double strength{1.0};
    bool is_stunned{false};
    bool is_invulnerable{false};
};

struct CharInv final {
    std::vector<CursedTool>   inventory;
    std::optional<CursedTool> stored_tool{};
    std::optional<CursedTool> current_tool{};
    bool has_access_to_inventory{false};
};

class Character {
protected:
    EntityInfo identity;
    BattleIQ style;
    CharState state;
    CharInv equipment;
public:
    Character() {};
    virtual ~Character();

    [[nodiscard]] const EntityInfo& Identity() const noexcept;
    [[nodiscard]] const BattleIQ& Style() const noexcept;
    [[nodiscard]] const CharState& State() const noexcept;
    [[nodiscard]] const CharInv& Equipment() const noexcept;

    EntityInfo& Identity() noexcept;
    BattleIQ& Style() noexcept;
    CharState& State() noexcept;
    CharInv& Equipment() noexcept;

    [[nodiscard]] std::string Name() const noexcept;

    [[nodiscard]] double Health(ValType type = ValType::Current) const noexcept;
    void Health(OpType type, double amount);

    DamageStruct Damage(double amount, globalums::DamageType dmg_type = globalums::DamageType::Normal);
    AttackStruct Attack(Character& attacked);

    [[nodiscard]] virtual const CurseUser* CanUseSorcery() const noexcept;
    virtual CurseUser* CanUseSorcery() noexcept;
};