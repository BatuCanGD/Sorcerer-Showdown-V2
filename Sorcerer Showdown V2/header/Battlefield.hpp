#pragma once
#include "CharacterType/Character.hpp"

#include <memory>
#include <vector>

struct battlefield {
    std::vector<std::unique_ptr<Character>> battlefield;
    std::vector<std::unique_ptr<Character>> spawn_next;
};