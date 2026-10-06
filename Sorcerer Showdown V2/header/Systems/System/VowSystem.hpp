#pragma once
#include "../../Sorcery/BindingVow.hpp"
class CurseUser;
struct EntityInfo;
struct BattleIQ;
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
    void ApplyVows(CurseUser& c);
    bool DoVow(CurseUser& c, const VowCostSystem& vcs);
    const VowCostSystem GetVowCost(const CurseUser& c, const BindingVow& bv);
    const EntityInfo GetRandomId(const BindingVow& bv);
    const BindingVow CreateVow(CurseUser& j, const BattleIQ& bq);
}