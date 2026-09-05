#pragma once

#include <memory>

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
    [[nodiscard]] std::unique_ptr<Technique> Limitless();
    [[nodiscard]] std::unique_ptr<Technique> IdleTransfiguration();
}