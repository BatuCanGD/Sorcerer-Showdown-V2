#pragma once

#include <memory>

class Character;
class CurseUser;

class Technique;

namespace Create {
    // base character class
    [[nodiscard]] std::unique_ptr<Character> TranfiguredHuman();

    // curse user class
    [[nodiscard]] std::unique_ptr<CurseUser> Mahito();
    [[nodiscard]] std::unique_ptr<CurseUser> Gojo();

    // technique class
    [[nodiscard]] Technique Limitless();
    [[nodiscard]] Technique IdleTransfiguration();
}