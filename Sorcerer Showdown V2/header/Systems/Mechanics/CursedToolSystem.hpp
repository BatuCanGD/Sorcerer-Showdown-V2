#pragma once
#include <cstdint>
struct CursedTool;
struct CharInv;

namespace CursedToolSystem {
    enum class WeaponPlacement : std::uint8_t { Hand, Offhand, Inventory };
    enum class WeaponChoice : std::uint8_t { Unequip, None, Random, HighestDamage, EffectInducing, TechniqueBypassing };
    CursedTool GetFromInv(CharInv& inv, WeaponChoice wc);
    CursedTool GetWeapon(CharInv& inv, WeaponPlacement which, WeaponChoice which_type);
    void MoveWeapon(CharInv& inv, CursedTool wp, WeaponPlacement where, WeaponChoice wc);
    bool HasWeapon(CharInv& c,WeaponChoice wc);
    void SwitchWeapons(CharInv& inv, WeaponPlacement which, WeaponPlacement where, WeaponChoice wc = WeaponChoice::None);
    void SwitchTo(CharInv& c, WeaponChoice wc);
}