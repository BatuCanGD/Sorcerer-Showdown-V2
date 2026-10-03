#pragma once

#include "Stuff/StatusEffects.hpp"
#include <memory>

class Character;
class CurseUser;
class Technique;

struct CursedTool;
struct Domain;
struct Neutralizer;
struct StatusEffect;

namespace Create {
    // characters
    [[nodiscard]] std::unique_ptr<Character> TranfiguredHuman();
    [[nodiscard]] std::unique_ptr<CurseUser> Mahito();
    [[nodiscard]] std::unique_ptr<CurseUser> Gojo();

    // techniques
    [[nodiscard]] Technique Limitless();
    [[nodiscard]] Technique IdleTransfiguration();

    // domains
    [[nodiscard]] Domain UnlimitedVoid();
    [[nodiscard]] Domain MalevolentShrine();
    [[nodiscard]] Domain SelfEmbodimentOfPerfection();

    // neutralizers
    [[nodiscard]] Neutralizer SimpleDomain();
    [[nodiscard]] Neutralizer FallingBlossomEmotion();

    // cursed tools
    [[nodiscard]] CursedTool InvertedSpearOfHeaven();

    // effects
    [[nodiscard]] StatusEffect BleedEffect();
}