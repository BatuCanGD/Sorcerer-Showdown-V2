#include "../header/AI.hpp"
#include "../header/Logger.hpp"
#include "../header/Systems/Mechanics/CombatSystem.hpp"
#include "../header/CharacterType/CurseUser.hpp"
#include "../header/Systems/Mechanics/CursedToolSystem.hpp"
#include "../header/Systems/GenericSystems.hpp"
#include "../header/Systems/Mechanics/VowSystem.hpp"
#include "../header/Systems/Mechanics/TechniqueSystem.hpp"
#include "../header/Systems/Mechanics/SorcerySystem.hpp"
#include "../header/Utilities/Random.hpp"
#include "../header/Battlefield.hpp"

#include <limits>
#include <print>
#include <stdexcept>

Character* AI::GetTarget(const Character& user, const TargetingType& tp, const Battlefield &bf){
    if (bf.battlefield.size() <= 1){
        throw std::runtime_error("0-1 Entities left in vector, Impossible to find target");
    }
    switch(tp){
        case TargetingType::HighestHP: {
            Character* target{nullptr};
            double highest_hp{std::numeric_limits<double>::lowest()};
            for (const auto& c : bf.battlefield){
                if (c.get() == &user || c->State().health <= 0.0) continue;
                if (c->State().health > highest_hp || !target){
                    target = c.get();
                    highest_hp = c->State().health;
                }
            }
            return target;
        }
        case TargetingType::LowestHP: {
            Character* target{nullptr};
            double lowest_hp{std::numeric_limits<double>::max()};
            for (const auto& c : bf.battlefield){
                if (c.get() == &user || c->State().health <= 0.0) continue;
                if (c->State().health < lowest_hp || !target){
                    target = c.get();
                    lowest_hp = c->State().health;
                }
            }
            return target;
        }
        case TargetingType::Mixed: {
            Character* target{nullptr};
            do {
                target = bf.battlefield[get_random<size_t>(0, bf.battlefield.size() - 1)].get();
            }while(target->State().health <= 0.0 || target == &user);
            return target;
        }
    }
    return nullptr;
}

void AI::DoFighting(Character& user, Character* target, const Battlefield& bf, const BattleIQ& iq) { // possible technique/domain use
    if (!target){
        throw std::runtime_error("No target exists");
    }
    if (auto* c = user.CanUseSorcery()){
        AI::DF_UseDomain(*c, bf, iq.fighting_style);
        AI::DF_UseTechnique(*c, *target, iq);
    }
    const auto l = user.Attack(*target);
    Log::Attack(l, user, *target);
}
void AI::DoResourceManagement(Character& user, const Character& tr,const Battlefield& bf ,const ResourceUsage& ru) {
    AI::DRM_UseTools(user, tr);
    if (auto* c = user.CanUseSorcery()){
        AI::DRM_UseReinforcement(*c, ru);
        AI::DRM_UseShikigami(*c, ru);
        AI::DRM_UseBindingVow(*c, tr, bf, ru);
        AI::DRM_UseRCT(*c, ru);
    }
}

void AI::DF_UseDomain(CurseUser &c, const Battlefield &bf, const FightingStyle& fs) {
    size_t domain_users{0};
    for (const auto& cc : bf.battlefield){
        if (const auto* cr = cc->CanUseSorcery()) {
            if (cr->Jujutsu().domain && cr->Jujutsu().domain->is_active) domain_users++;
        }
    }

    auto& domain = c.Jujutsu().domain;
    auto& neutral = c.Jujutsu().neutralizer;

    switch(fs){
        case FightingStyle::Aggressive: {
            if (domain) {
                const bool enough_ce = c.CursedEnergy().cursed_energy >= SorcerySystem::ApplySpendingMultiplier(domain->cost, c);
                if (!domain->is_active && enough_ce){
                    if (domain_users == 1 || get_random<std::uint8_t>(1, 100) >= 60) {
                        domain->is_active = true;
                    }
                }else if (domain->is_active && !enough_ce && get_random<std::uint8_t>(1, 25) >= 15){
                    domain->is_active = false;
                }
            }
            if (neutral) {
                const bool enough_ce = c.CursedEnergy().cursed_energy >= SorcerySystem::ApplySpendingMultiplier(neutral->cost, c);
                if (enough_ce){
                    if ((domain && !domain->is_active) || domain_users == 1) {
                        neutral->is_active = true;
                    }
                }else if (neutral->is_active) {
                    neutral->is_active = false;
                }
            }
            break;
        }
        case FightingStyle::Defensive: {
            if (domain) {
                const bool enough_ce = c.CursedEnergy().cursed_energy >= SorcerySystem::ApplySpendingMultiplier(domain->cost, c);
                if (!domain->is_active && enough_ce){
                    if (domain_users == 1 || get_random<std::uint8_t>(1, 100) >= 60) {
                        domain->is_active = true;
                    }
                }else if (domain->is_active && !enough_ce && get_random<std::uint8_t>(1, 25) >= 3){
                    domain->is_active = false;
                }
            }
            if (neutral) {
                const bool enough_ce = c.CursedEnergy().cursed_energy >= SorcerySystem::ApplySpendingMultiplier(neutral->cost, c);
                if (enough_ce){
                    if ((domain && !domain->is_active) || domain_users == 1) {
                        neutral->is_active = true;
                    }
                }else if (neutral->is_active) {
                    neutral->is_active = false;
                }
            }
            break;
        }
        case FightingStyle::Mixed: {
            if (domain) {
                const bool enough_ce = c.CursedEnergy().cursed_energy >= SorcerySystem::ApplySpendingMultiplier(domain->cost, c);
                if (enough_ce){
                    if (!domain->is_active && (domain_users == 1  || get_random<std::uint8_t>(1, 50) >= 25)) {
                        domain->is_active = true;
                    }
                }else if (domain->is_active && !enough_ce && get_random<std::uint8_t>(1, 25) >= 9){
                    domain->is_active = false;
                }
            }
            if (neutral) {
                const bool enough_ce = c.CursedEnergy().cursed_energy >= SorcerySystem::ApplySpendingMultiplier(neutral->cost, c);
                if (enough_ce){
                    if ((domain && !domain->is_active) || domain_users == 1) {
                        neutral->is_active = true;
                    }
                }else if (neutral->is_active) {
                    neutral->is_active = false;
                }
            }
            break;
        }
    }
}

void AI::DF_UseTechnique(CurseUser &c, Character &target, const BattleIQ &iq) {
    auto& tech = c.Jujutsu().technique;
    if (!tech){
        return;
    }
    auto* ab = TechniqueSystem::GetAbility(*tech, static_cast<TechniqueSystem::AbilityType>(get_random<std::uint8_t>(0, 3)));
    const bool enough_ce = c.CursedEnergy().cursed_energy >= SorcerySystem::ApplySpendingMultiplier(ab->cost, c);
    
    if (!enough_ce){
        return;
    }

    int c_amount{};
    switch(iq.fighting_style){
        case FightingStyle::Aggressive:{
            c_amount = 33;
            break;
        }
        case FightingStyle::Defensive:{
            c_amount = 67;
            break;
        }
        case FightingStyle::Mixed:{
            c_amount = get_random<int>(1, 100);
            break;
        }
    }
    while(get_random<int>(1, 100) >= c_amount && ab->chant_amount < 5) {
        TechniqueSystem::ChantAbility(*ab, c);
    }

    Log::TechniqueAttack(CombatSystem::ResolveTechnique(c, *ab, target));
}
void AI::DRM_UseBindingVow(CurseUser &c, const Character &tr, const Battlefield &bf, const ResourceUsage &iq) {
    auto& vows = c.Jujutsu().binding_vows;
    size_t size{0};
    switch(iq){
        case ResourceUsage::AllOut:
            size = 3;
            break;
        case ResourceUsage::Conservative:
            size = 1;
            break;
        case ResourceUsage::Mixed:
            size = 2;
            break;
        default:
            size = 0;
    }
    if (vows.size() < size){
        vows.push_back(VowSystem::CreateVow(c, c.Style()));
    }
}
void AI::DRM_UseShikigami(CurseUser &c, const ResourceUsage &rs){
    if (c.Jujutsu().shikigami.empty()) return;
    SummonType type{};
    switch(rs){
        case ResourceUsage::AllOut: {     
            if (SorcerySystem::CEMoreThanMax(c, 0.45)){
                type = SummonType::Active;  
            }else if (SorcerySystem::CEMoreThanMax(c, 0.15)){
                type = SummonType::Support;
            }else{
                type = SummonType::Shadow;
            }
            break;
        }
        case ResourceUsage::Mixed: {
            if (SorcerySystem::CEMoreThanMax(c, get_random<double>(0.30, 0.60))){
                type = SummonType::Active;  
            }else if (SorcerySystem::CEMoreThanMax(c, get_random<double>(0.05, 0.30))){
                type = SummonType::Support;
            }else{
                type = SummonType::Shadow;
            }
            break;
        }
        case ResourceUsage::Conservative: {
            if (SorcerySystem::CEMoreThanMax(c, 0.65)){
                type = SummonType::Active;  
            }else if (SorcerySystem::CEMoreThanMax(c, 0.40)){
                type = SummonType::Support;
            }else{
                type = SummonType::Shadow;
            }
            break;
        }
    }
    for(auto& s : c.Jujutsu().shikigami){
        s.summon_type = type;
    }
}
void AI::DRM_UseReinforcement(CurseUser &c, const ResourceUsage &rs) {
    double val{0.0};
    switch(rs){
        case ResourceUsage::AllOut:
            if (SorcerySystem::CEMoreThanMax(c, 0.40) && !GenericSystems::HPMoreThanMax(c, 0.40)){
                val = 135.0;
            }else if (SorcerySystem::CEMoreThanMax(c, 0.20) && !GenericSystems::HPMoreThanMax(c, 0.33)){
                val = 90.0;
            }else if (!GenericSystems::HPMoreThanMax(c, 0.25)){
                val = 60.0;
            }
            break;
        case ResourceUsage::Mixed:
            if (SorcerySystem::CEMoreThanMax(c, get_random<double>(0.25, 0.75)) && c.State().health < c.State().max_health * get_random<double>(0.60, 0.90)){
                val = get_random<double>(70.0, 150.0);
            }else if (SorcerySystem::CEMoreThanMax(c, 0.20) && c.State().health < c.State().max_health * get_random<double>(0.30, 0.60)){
                val = get_random<double>(65.0, 100.0);
            }else if (c.State().health < c.State().max_health * get_random<double>(0.01, 0.30)){
                val = get_random<double>(20.0, 80.0);
            }
            break;
        case ResourceUsage::Conservative:
            if (SorcerySystem::CEMoreThanMax(c, 0.60) && !GenericSystems::HPMoreThanMax(c, 0.55)){
                val = 90.0;
            }else if (SorcerySystem::CEMoreThanMax(c, 0.40) && !GenericSystems::HPMoreThanMax(c, 0.35)){
                val = 50.0;
            }else if (!GenericSystems::HPMoreThanMax(c, 0.30)){
                val = 35.0;
            }
            break;
    }
    c.CursedEnergy().reinforcement_amount = val;
}
void AI::DRM_UseRCT(CurseUser &c, const ResourceUsage &rs) {
    if (!c.RCT().can_use_rct){
        return;
    }
    double val{0.0};
    switch(rs){
        case ResourceUsage::AllOut:
            if (SorcerySystem::CEMoreThanMax(c, 0.40) && !GenericSystems::HPMoreThanMax(c, 0.40)){
                val = 115.0;
            }else if (SorcerySystem::CEMoreThanMax(c, 0.20) && !GenericSystems::HPMoreThanMax(c, 0.33)){
                val = 85.0;
            }else if (!GenericSystems::HPMoreThanMax(c, 0.25)){
                val = 65.0;
            }
            break;
        case ResourceUsage::Mixed:
            if (SorcerySystem::CEMoreThanMax(c, get_random<double>(0.25, 0.75)) && !GenericSystems::HPMoreThanMax(c, get_random<double>(0.60, 0.90))){
                val = get_random<double>(50.0, 150.0);
            }else if (SorcerySystem::CEMoreThanMax(c, 0.20) && !GenericSystems::HPMoreThanMax(c, get_random<double>(0.30, 0.60))){
                val = get_random<double>(35.0, 80.0);
            }else if (!GenericSystems::HPMoreThanMax(c, get_random<double>(0.01, 0.30))){
                val = get_random<double>(10.0, 50.0);
            }
            break;
        case ResourceUsage::Conservative:
            if (SorcerySystem::CEMoreThanMax(c, 0.60) && !GenericSystems::HPMoreThanMax(c, 0.55)){
                val = 100.0;
            }else if (SorcerySystem::CEMoreThanMax(c, 0.40) && !GenericSystems::HPMoreThanMax(c, 0.35)){
                val = 55.0;
            }else if (!GenericSystems::HPMoreThanMax(c, 0.30)){
                val = 50.0;
            }
            break;
    }
    c.RCT().rct_output = val;
}
void AI::DRM_UseTools(Character &c, const Character & tr) {
    auto& eq = c.Equipment();
    if ((!eq.current_tool && !eq.stored_tool && (!eq.has_access_to_inventory || eq.inventory.empty()))){
        return;
    }
    if (const auto& cr = tr.CanUseSorcery()){
        if (const auto& t = cr->Jujutsu().technique) {
            if (TechniqueSystem::HasBarrier(*t)){
                CursedToolSystem::SwitchTo(eq, CursedToolSystem::WeaponChoice::TechniqueBypassing);
                return;
            }
        }
    }
    if (tr.State().health > c.State().health || GenericSystems::HPMoreThanMax(c, 0.70)){
        SwitchTo(eq, CursedToolSystem::WeaponChoice::HighestDamage);
    }
}

void AI::Fight(Character &user, Battlefield &bf) {
    const auto& style{user.Style()};

    try {
        Character* target = AI::GetTarget(user, style.targeting_type, bf);
        DoResourceManagement(user, *target, bf, style.resource_usage);
        DoFighting(user, target, bf, style);
    }catch(const std::out_of_range& oor) {
        std::println(stderr, "Range error: {}", oor.what());
    }catch(const std::runtime_error& re){
        std::println(stderr, "Runtime error: {}", re.what());
    }
}