#pragma once
#include "../../Sorcery/BindingVow.hpp"
#include <vector>

class CurseUser;

struct VowCostSystem final {
    const BindingVow* bv;
    double sacrifice;
    double gain;
};

namespace VowSystem {
    void ApplyVows(CurseUser& c);
    bool DoVow(CurseUser& c, const VowCostSystem& vcs);
    VowCostSystem GetVowCost(const CurseUser& c, const BindingVow& bv);
}