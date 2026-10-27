#pragma once
#include <variant>

#include "BaseStruct.hpp"
#include "SorceryStruct.hpp"

struct Character;
struct Technique;
struct CursedTool;
struct BindingVow;
struct Domain;
struct Shikigami;
struct Battlefield;

namespace AI {
    std::variant<Character*, Shikigami*> GetTarget(const Character& user, const Base::DecisionMaker& tp, const Battlefield& bf);

    void DF_Attack(Character& c, std::variant<Character*, Shikigami*> target);
    void DF_UseDomain(Character& c, const Battlefield& bf, const FightingStyle& fs);
    void DF_UseTechnique(Character& c, std::variant<Character*, Shikigami*> target, const Base::DecisionMaker& bq);

    void DRM_UseShikigami(Character& c, const ResourceUsage& rs);
    void DRM_UseBindingVow(Character& c, const std::variant<Character*, Shikigami*> tr, const Battlefield& bf, const ResourceUsage& iq);
    void DRM_UseReinforcement(Character& c, const ResourceUsage& rs);
    void DRM_UseRCT(Character& c, const ResourceUsage& rs);
    void DRM_UseTools(Character& c, const std::variant<Character*, Shikigami*> tr);

    void DoFighting(Character& user, std::variant<Character*, Shikigami*> target, const Battlefield& bf, const Base::DecisionMaker& iq);
    void DoResourceManagement(Character& user, const std::variant<Character*, Shikigami*> tr ,const Battlefield& bf, const ResourceUsage& ru);

    void Fight(Character& user, Battlefield& bf);
};