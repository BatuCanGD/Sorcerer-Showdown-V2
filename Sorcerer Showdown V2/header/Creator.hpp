#pragma once
class Character;
class CurseUser;
struct Technique;
struct CursedTool;
struct Shikigami;
struct StatusEffect;
struct Domain;
struct Neutralizer;
struct StatusEffect;

namespace Create {
    namespace Char {
        [[nodiscard]] Character TranfiguredHuman();
        [[nodiscard]] Character Toji();
        [[nodiscard]] Character Maki();
        [[nodiscard]] CurseUser Mahito();
        [[nodiscard]] CurseUser Jogo();
        [[nodiscard]] CurseUser Hanami();
        [[nodiscard]] CurseUser Gojo();
        [[nodiscard]] CurseUser Sukuna();
    }
    namespace Tech {
        [[nodiscard]] Technique Limitless();
        [[nodiscard]] Technique IdleTransfiguration();
        [[nodiscard]] Technique Shrine();
        [[nodiscard]] Technique DisasterFlames();
        [[nodiscard]] Technique DisasterPlants();
    }
    namespace Dmn {
        [[nodiscard]] Domain UnlimitedVoid();
        [[nodiscard]] Domain MalevolentShrine();
        [[nodiscard]] Domain SelfEmbodimentOfPerfection();
        [[nodiscard]] Domain ShiningSeaOfGrowingBranches();
        [[nodiscard]] Domain CoffinOfTheIronMountain();
    }
    namespace Nlr {
        [[nodiscard]] Neutralizer SimpleDomain();
        [[nodiscard]] Neutralizer HollowWickerBasket();
        [[nodiscard]] Neutralizer FallingBlossomEmotion();
    }
    namespace Shk {
        [[nodiscard]] Shikigami Mahoraga();
        [[nodiscard]] Shikigami Agito();
        [[nodiscard]] Shikigami EmberInsects();
    }
    namespace Tool {
        [[nodiscard]] CursedTool InvertedSpearOfHeaven();
        [[nodiscard]] CursedTool SplitSoulKatana();
        [[nodiscard]] CursedTool PlayfulCloud();
    }
    namespace Effect {
        [[nodiscard]] StatusEffect BleedEffect();
    }
}