#pragma once
#include <vector>
#include <memory>

struct Character;

struct Battlefield final {
    std::vector<std::unique_ptr<Character>> battlefield;
    std::vector<std::unique_ptr<Character>> spawn_next;
};