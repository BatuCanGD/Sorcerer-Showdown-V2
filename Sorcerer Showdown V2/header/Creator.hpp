#pragma once

#include <memory>
#include <optional>

class Character;
class CurseUser;
class Sorcerer;

class Technique;

namespace Create {
    // base character class
    [[nodiscard]] std::unique_ptr<Character> TranfiguredHuman();

    // curse user class
    [[nodiscard]] std::unique_ptr<CurseUser> Mahito();

    // sorcerer class
    [[nodiscard]] std::unique_ptr<Sorcerer> Gojo();

    // technique class
    [[nodiscard]] std::optional<Technique> Limitless();
    [[nodiscard]] std::optional<Technique> IdleTransfiguration();
}