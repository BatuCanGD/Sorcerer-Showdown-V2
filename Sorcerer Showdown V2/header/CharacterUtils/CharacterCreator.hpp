#pragma once

#include <memory>

class Character;
class CurseUser;
class Sorcerer;

namespace Create {
    // base character class
    [[nodiscard]] std::unique_ptr<Character> TranfiguredHuman();
    // curse user class
    [[nodiscard]] std::unique_ptr<CurseUser> Mahito();
    // sorcerer class
    [[nodiscard]] std::unique_ptr<Sorcerer> Gojo();
}