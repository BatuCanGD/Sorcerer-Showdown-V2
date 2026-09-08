#pragma once

class Technique;
struct TechAbility;
struct TechIdentity;

struct TechniqueEditor final {
    static void SetIdentity(Technique& t, TechIdentity id);
    static void AddAbility(Technique& t, TechAbility& tb);
};