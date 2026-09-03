#pragma once

#include <memory>
#include <vector>

class Character;

struct battlefield {
    std::vector<std::unique_ptr<Character>> battlefield;
    std::vector<std::unique_ptr<Character>> spawn_next;
};