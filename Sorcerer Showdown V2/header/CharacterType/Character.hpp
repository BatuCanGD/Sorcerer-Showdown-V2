#pragma once

#include "../Stuff/CursedTool.hpp"
#include "../Enums.hpp"
#include "../Structs.hpp"

#include <string>
#include <optional>
#include <vector>

class CurseUser;
struct AttackStruct;
struct DamageStruct;

struct CharCtrl final {
    bool is_player{false};
};

struct CharState final {
    double health{1.0};
    double max_health{1.0};
    double durability{1.0};
    double strength{1.0};
    bool is_stunned{false};
    bool is_invulnerable{false};
};

struct CharInv final {
    std::vector<WeaponType>   inventory;
    std::optional<WeaponType> stored_tool{};
    std::optional<WeaponType> current_tool{};
    bool has_access_to_inventory{false};
};

class Character {
    friend struct CharacterEditor;
    friend struct CombatSystem;
protected:
    EntityInfo identity;
    CharCtrl control;
    CharState state;
    CharInv equipment;
public:
    Character() {};
    virtual ~Character();

    [[nodiscard]] const EntityInfo& Identity() const noexcept;
    [[nodiscard]] const CharCtrl& Control() const noexcept;
    [[nodiscard]] const CharState& State() const noexcept;
    [[nodiscard]] const CharInv& Equipment() const noexcept;

    EntityInfo& Identity() noexcept;
    CharCtrl& Control() noexcept;
    CharState& State() noexcept;
    CharInv& Equipment() noexcept;

    [[nodiscard]] std::string Name() const noexcept;

    [[nodiscard]] double Health(ValType type = ValType::Current) const noexcept;
    void Health(OpType type, double amount);

    DamageStruct Damage(double amount, globalums::DamageType dmg_type = globalums::DamageType::Normal);
    AttackStruct Attack(Character& attacked);

    [[nodiscard]] virtual const CurseUser* CanUseSorcery() const noexcept;
};