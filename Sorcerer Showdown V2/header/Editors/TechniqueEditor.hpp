#pragma once

class Technique;
struct EntityInfo;
struct TechAbility;
struct TechIdentity;

namespace TechniqueEditor {
    void SetIdentity(Technique& t, EntityInfo id);
    void SetBarrier(Technique& t, bool bl);
    void AddAbility(Technique& t, TechAbility tb);
};