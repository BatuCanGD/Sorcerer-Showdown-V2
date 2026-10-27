#include "../../header/Creator.hpp"
#include "../../header/Sorcery/Neutralizer.hpp"

Neutralizer Create::Nlr::SimpleDomain() {
    Neutralizer c{};
    c.identity = {"Simple Domain", "\x1b[33m", "A barrier technique that creates a small zone to neutralize cursed techniques"};
    c.neutralizer_type = NeutralizerType::FullyProtected;
    c.cost = 100.0;
    c.max_health = 200.0;
    c.health = c.max_health;
    c.durability = 200.0;
    return c;
}
Neutralizer Create::Nlr::HollowWickerBasket() {
    Neutralizer c{};
    c.identity = {"Hollow Wicker Basket", "\x1b[33m", "A barrier technique older than simple domain that requires continuous hand signs"};
    c.neutralizer_type = NeutralizerType::FullyProtected;
    c.cost = 300.0;
    c.max_health = 300.0;
    c.health = c.max_health;
    c.durability = 75.0;
    return c;
}
Neutralizer Create::Nlr::FallingBlossomEmotion() {
    Neutralizer c{};
    c.identity = {"Falling Blossom Emotion", "\x1b[32m", "An anti-domain technique that automatically counters incoming attacks using cursed energy"};
    c.cost = 50.0;
    c.max_health = 50.0;
    c.health = c.max_health;
    c.durability = 50.0;
    c.neutralizer_type = NeutralizerType::ReducedDamage;
    return c;
}