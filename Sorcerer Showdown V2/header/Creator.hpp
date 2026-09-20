#pragma once

#include "Stuff/StatusEffects.hpp"
#include <memory>

class Character;
class CurseUser;
class Technique;

struct CursedTool;
struct StatusEffect;

namespace Create {
    // characters
    [[nodiscard]] std::unique_ptr<Character> TranfiguredHuman();
    [[nodiscard]] std::unique_ptr<CurseUser> Mahito();
    [[nodiscard]] std::unique_ptr<CurseUser> Gojo();

    // technique class
    [[nodiscard]] Technique Limitless();
    [[nodiscard]] Technique IdleTransfiguration();

    // cursed tools
    [[nodiscard]] CursedTool InvertedSpearOfHeaven();

    // effects
    [[nodiscard]] StatusEffect BleedEffect();
}