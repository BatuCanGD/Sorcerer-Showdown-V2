#pragma once
#include "../../BaseStruct.hpp"
#include <cstdint>
struct CursedTool;

namespace CursedToolSystem {
    enum class WeaponPlacement : std::uint8_t { Hand, Offhand, Inventory };
    enum class WeaponChoice : std::uint8_t { Unequip, None, Random, HighestDamage, EffectInducing, TechniqueBypassing };
    CursedTool GetFromInv(Base::Equipment& inv, WeaponChoice wc);
    CursedTool GetWeapon(Base::Equipment& inv, WeaponPlacement which, WeaponChoice which_type);
    void MoveWeapon(Base::Equipment& inv, CursedTool wp, WeaponPlacement where, WeaponChoice wc);
    bool HasWeapon(Base::Equipment& c,WeaponChoice wc);
    void SwitchWeapons(Base::Equipment& inv, WeaponPlacement which, WeaponPlacement where, WeaponChoice wc = WeaponChoice::None);
    void SwitchTo(Base::Equipment& c, WeaponChoice wc);
}