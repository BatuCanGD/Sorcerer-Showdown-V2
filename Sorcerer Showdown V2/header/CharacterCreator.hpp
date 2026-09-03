#pragma once

#include <memory>

class Character;
class CurseUser;
class Sorcerer;

namespace Create {
    [[nodiscard]] std::unique_ptr<Character> TranfiguredHuman();
    [[nodiscard]] std::unique_ptr<CurseUser> Mahito();
    [[nodiscard]] std::unique_ptr<Sorcerer> Gojo();
}