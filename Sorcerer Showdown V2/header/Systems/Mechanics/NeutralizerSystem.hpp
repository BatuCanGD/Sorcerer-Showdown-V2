#pragma once

class Character;
class CurseUser;
struct Neutralizer;

namespace NeutralizerSystem {
    void HandleLifeTime(CurseUser& c, Neutralizer& n);
    void Damage(Neutralizer& c, const double damage);
    void Reset(Neutralizer& c);
}