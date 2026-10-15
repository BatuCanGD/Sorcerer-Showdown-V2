#pragma once
#include <nlohmann/json.hpp>
#include <memory>
class Character;
class CurseUser;
class Technique;
struct CursedTool;
struct Shikigami;
struct StatusEffect;
struct Domain;
struct Neutralizer;
struct StatusEffect;

inline nlohmann::json CheckAndGetObject(const nlohmann::json& j, std::string_view name){
    return (j.contains(name) && j.at(name).is_object()) ? j.at(name) : j;
}

namespace Create {
    namespace Char {
        [[nodiscard]] std::unique_ptr<Character> Custom(const nlohmann::json& j);
        [[nodiscard]] Character TranfiguredHuman();
        [[nodiscard]] Character Toji();
        [[nodiscard]] Character Maki();
        [[nodiscard]] CurseUser Mahito();
        [[nodiscard]] CurseUser Gojo();
        [[nodiscard]] CurseUser Sukuna();
    }
    namespace Tech {
        [[nodiscard]] Technique Limitless();
        [[nodiscard]] Technique IdleTransfiguration();
        [[nodiscard]] Technique Shrine();
    }
    namespace Dmn {
        [[nodiscard]] Domain UnlimitedVoid();
        [[nodiscard]] Domain MalevolentShrine();
        [[nodiscard]] Domain SelfEmbodimentOfPerfection();
    }
    namespace Nlr {
        [[nodiscard]] Neutralizer SimpleDomain();
        [[nodiscard]] Neutralizer HollowWickerBasket();
        [[nodiscard]] Neutralizer FallingBlossomEmotion();
    }
    namespace Shk {
        [[nodiscard]] Shikigami Mahoraga();
        [[nodiscard]] Shikigami Agito();
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