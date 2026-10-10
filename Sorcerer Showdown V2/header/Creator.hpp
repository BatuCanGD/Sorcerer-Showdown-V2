#pragma once
class Character;
class CurseUser;
class Technique;
struct CursedTool;
struct Shikigami;
struct StatusEffect;
struct Domain;
struct Neutralizer;
struct StatusEffect;

namespace Create {
    // characters
    [[nodiscard]] Character TranfiguredHuman();
    [[nodiscard]] CurseUser Mahito();
    [[nodiscard]] CurseUser Gojo();
    [[nodiscard]] CurseUser Sukuna();

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

    // shikigami
    [[nodiscard]] Shikigami Mahoraga();
    [[nodiscard]] Shikigami Agito();

    // cursed tools
    [[nodiscard]] CursedTool InvertedSpearOfHeaven();

    // effects
    [[nodiscard]] StatusEffect BleedEffect();
}