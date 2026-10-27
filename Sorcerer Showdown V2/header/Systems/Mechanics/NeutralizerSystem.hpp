#pragma once

struct Character;

struct Neutralizer;

namespace NeutralizerSystem {
    void HandleLifeTime(Character& c, Neutralizer& n);
    void Damage(Neutralizer& c, const double damage);
    void Reset(Neutralizer& c);
}