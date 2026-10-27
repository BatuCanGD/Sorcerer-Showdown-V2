#pragma once
#include "../../Sorcery/BindingVow.hpp"
#include "../../BaseStruct.hpp"
struct Character;
struct EntityInfo;
struct JujutsuSystem;

struct VowCostSystem final {
    const BindingVow* bv;
    double sacrifice;
    double gain;
};

struct VowType final {
    SacrificeType sack;
    SacrificeType gain;
};

namespace VowSystem {
    void ApplyVows(Character& c);
    bool DoVow(Character& c, const VowCostSystem& vcs);
    const VowCostSystem GetVowCost(const Character& c, const BindingVow& bv);
    const EntityInfo GenerateVowId(const BindingVow& bv);
    const BindingVow CreateVow(Character& j, const Base::DecisionMaker& bq);
}