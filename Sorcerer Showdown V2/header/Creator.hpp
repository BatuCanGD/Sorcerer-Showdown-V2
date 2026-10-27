#pragma once
struct Character;
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
        [[nodiscard]] Character Mahito();
        [[nodiscard]] Character Jogo();
        [[nodiscard]] Character Hanami();
        [[nodiscard]] Character Gojo();
        [[nodiscard]] Character Sukuna();
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
        [[nodiscard]] StatusEffect Bleed();
    }
}