#pragma once

class Character;
struct Neutralizer;

namespace NeutralizerSystem {
    void HandleDamage(Neutralizer& c, const double damage);
    void ResetNeutralizer(Neutralizer& c);
}