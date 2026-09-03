#include "../header/CharacterCreator.hpp"
#include "../header/CharacterEditor.hpp"
#include "../header/Utilities/Random.hpp"
#include "../header/CharacterType/Character.hpp"
#include "../header/CharacterType/CurseUser.hpp"
#include "../header/CharacterType/Sorcerer.hpp"

#include <memory>

// character
std::unique_ptr<Character> Create::TranfiguredHuman() {
    auto tr = std::make_unique<Character>();
    Editor::SetName(*tr, "Transfigured Human");
    Editor::SetColor(*tr, "\x1b[91m");
    Editor::SetHealth(*tr, get_random<double>(1.0, 100.0));
    Editor::SetDurability(*tr, get_random<double>(1.0, 100.0));
    Editor::SetStrength(*tr, get_random<double>(1.0, 100.0));
    return tr;
}
// curse user
std::unique_ptr<CurseUser> Create::Mahito() {
    auto mh = std::make_unique<CurseUser>();
    double health = 550.0, cursed_energy = 4000.0, strength = 115.0;
    double durability = 75.0;
    Editor::SetName(*mh, "Mahito");
    Editor::SetColor(*mh, "\x1b[91m");
    Editor::SetHealth(*mh, health);
    Editor::SetDurability(*mh, durability);
    Editor::SetStrength(*mh, strength);
    Editor::SetCursedEnergy(*mh, cursed_energy);
    Editor::SetDomain(*mh, std::unique_ptr<int>()); // placeholder
    Editor::SetTechnique(*mh, std::unique_ptr<int>()); // placeholder
    Editor::SetCursedEnergyEfficiency(*mh, CurseUser::CEfficiency::Stable);
    Editor::SetTraitPassiveHealing(*mh, true);
    return mh;
}
// sorcerer
std::unique_ptr<Sorcerer> Create::Gojo() {
    auto gj = std::make_unique<Sorcerer>();
    double health = 1000.0, cursed_energy = 5000.0, strength = 185.0;
    double durability = 300.0;
    Editor::SetName(*gj, "Gojo");
    Editor::SetColor(*gj, "\x1b[91m");
    Editor::SetHealth(*gj, health);
    Editor::SetDurability(*gj, durability);
    Editor::SetStrength(*gj, strength);
    Editor::SetCursedEnergy(*gj, cursed_energy);
    Editor::SetDomain(*gj, std::unique_ptr<int>()); // placeholder
    Editor::SetDomainNullifier(*gj, std::unique_ptr<int>()); // placeholder
    Editor::SetTechnique(*gj, std::unique_ptr<int>()); // placeholder
    Editor::SetCursedEnergyEfficiency(*gj, CurseUser::CEfficiency::Extreme);
    Editor::SetTraitSixEyes(*gj, true);
    return gj;
}