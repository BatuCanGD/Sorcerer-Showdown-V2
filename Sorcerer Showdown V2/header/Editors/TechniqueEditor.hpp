#pragma once

class Technique;
struct EntityInfo;
struct TechAbility;
struct TechIdentity;

struct TechniqueEditor final {
    static void SetIdentity(Technique& t, EntityInfo id);
    static void SetBarrier(Technique& t, bool bl);
    static void AddAbility(Technique& t, TechAbility& tb);
};