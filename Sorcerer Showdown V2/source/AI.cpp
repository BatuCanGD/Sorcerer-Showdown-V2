#include "../header/AI.hpp"
#include "../header/Logger.hpp"
#include "../header/Systems/Mechanics/CombatSystem.hpp"
#include "../header/Character.hpp"
#include "../header/Systems/Mechanics/CursedToolSystem.hpp"
#include "../header/Systems/CharacterSystem.hpp"
#include "../header/Systems/Mechanics/VowSystem.hpp"
#include "../header/Systems/Mechanics/TechniqueSystem.hpp"
#include "../header/Systems/Mechanics/SorcerySystem.hpp"
#include "../header/Utilities/Random.hpp"
#include "../header/Battlefield.hpp"

#include <limits>
#include <stdexcept>
#include <variant>

std::variant<Character*, Shikigami*> AI::GetTarget(const Character& user, const Base::DecisionMaker& tp, const Battlefield &bf){
    if (bf.battlefield.size() <= 1){
        throw std::runtime_error("0-1 Entities left in vector, Impossible to find target");
    }
    std::variant<Character*, Shikigami*> target;
    switch(tp.targeting_type){
        case TargetingType::HighestHP: {
            double highest_hp{std::numeric_limits<double>::lowest()};
            switch(tp.targeted_type){
                default:
                    [[fallthrough]];
                case TargetedTypes::Character: {
                    for (const auto& c : bf.battlefield){
                        if (c.get() == &user || c->state.health <= 0.0) {
                            continue;
                        } 
                        if (c->state.health > highest_hp){
                            target = c.get();
                            highest_hp = c->state.health;
                        }
                    }
                    break;
                }
                case TargetedTypes::Mixed: {
                    std::vector<std::variant<Character*, Shikigami*>> targets;
                    for (const auto& c : bf.battlefield){
                        if (c.get() == &user || c->state.health <= 0.0) {
                            continue;
                        } 
                        if (c->state.health > highest_hp){
                            target = c.get();
                            highest_hp = c->state.health;
                        }
                    }
                    targets.push_back(target);
                    if (const auto& crs = std::get<Character*>(target)->curseUser) {
                        for(auto& s : crs->jujutsu.shikigami){
                            if (s.summon_type == SummonType::Active){
                                targets.push_back(const_cast<Shikigami*>(&s));
                            }
                        }
                    }
                    target = targets[get_random<size_t>(0, targets.size() - 1)];
                    break;
                }
            }
            return target;
        }
        case TargetingType::LowestHP: {
            double lowest_hp{std::numeric_limits<double>::max()};
            switch(tp.targeted_type){
                default:
                    [[fallthrough]];
                case TargetedTypes::Character:
                    for (const auto& c : bf.battlefield){
                        if (c.get() == &user || c->state.health <= 0.0) {
                            continue;
                        }
                        if (c->state.health < lowest_hp){
                            target = c.get();
                            lowest_hp = c->state.health;
                        }
                    }
                    break;
                case TargetedTypes::Mixed:
                    std::vector<std::variant<Character*, Shikigami*>> targets;
                    for (const auto& c : bf.battlefield){
                        if (c.get() == &user || c->state.health <= 0.0) {
                            continue;
                        }
                        if (c->state.health < lowest_hp){
                            target = c.get();
                            lowest_hp = c->state.health;
                        }
                    }
                    targets.push_back(target);
                    if (const auto& crs = std::get<Character*>(target)->curseUser) {
                        for(auto& s : crs->jujutsu.shikigami){
                            if (s.summon_type == SummonType::Active){
                                targets.push_back(const_cast<Shikigami*>(&s));
                            }
                        }
                    }
                    target = targets[get_random<size_t>(0, targets.size() - 1)];
                    break;
            }

            return target;
        }
        case TargetingType::Random: {
            switch(tp.targeted_type){
                case TargetedTypes::Character:
                    do {
                        target = bf.battlefield[get_random<size_t>(0, bf.battlefield.size() - 1)].get();
                    }while(std::get<Character*>(target) == &user);
                    break;
                case TargetedTypes::Mixed:
                    do {
                        target = bf.battlefield[get_random<size_t>(0, bf.battlefield.size() - 1)].get();
                    }while(std::get<Character*>(target) == &user);
                    std::vector<std::variant<Character*, Shikigami*>> targets;
                    targets.push_back(target);
                    if (auto& crs = std::get<Character*>(target)->curseUser) {
                        for(auto& s : crs->jujutsu.shikigami){
                            if (s.summon_type == SummonType::Active){
                                targets.push_back(&s);
                            }
                        }
                    }
                    target = targets[get_random<size_t>(0, targets.size() - 1)];
                    break;
            }
            return target;
        }
    }
    return target;
}

void AI::DoFighting(Character& user, std::variant<Character*, Shikigami*> target, const Battlefield& bf, const Base::DecisionMaker& iq) { // possible technique/domain use
    if (user.curseUser.has_value()){
        AI::DF_UseDomain(user, bf, iq.fighting_style);
        AI::DF_UseTechnique(user, target, iq);
    }
    AI::DF_Attack(user, target);
}
void AI::DoResourceManagement(Character& user, const std::variant<Character*, Shikigami*> tr,const Battlefield& bf ,const ResourceUsage& ru) {
    AI::DRM_UseTools(user, tr);
    if (user.curseUser.has_value()){
        AI::DRM_UseReinforcement(user, ru);
        AI::DRM_UseShikigami(user, ru);
        AI::DRM_UseBindingVow(user, tr, bf, ru);
        AI::DRM_UseRCT(user, ru);
    }
}

void AI::DF_Attack(Character &c, std::variant<Character*, Shikigami*> target) {
    if (std::holds_alternative<Character*>(target)) {
        auto& tr = std::get<Character*>(target);
        if (auto& chr = c.curseUser) {
            if (chr->amplification.can_use_amp){
                bool& amp = chr->amplification.is_active;
                if (const auto& cc = tr->curseUser){
                    if (const auto& tech = cc->jujutsu.technique){
                        if (tech->barrier.can_use_barrier && tech->barrier.is_active) {
                            amp = true;
                        }else if (amp){
                            amp = false;
                        }
                    }
                }
            } // this is scarily visible on the visual studio scroll bar
        }
    }
    Log::Attack(CharacterSystem::Attack(c,target), c, target);
}

void AI::DF_UseDomain(Character &chr, const Battlefield &bf, const FightingStyle& fs) {
    size_t domain_users{0};
    for (const auto& cc : bf.battlefield){
        if (const auto& cr = cc->curseUser) {
            if (cr->jujutsu.domain && cr->jujutsu.domain->is_active) domain_users++;
        }
    }

    auto& c = chr.curseUser;
    auto& domain = c->jujutsu.domain;
    auto& neutral = c->jujutsu.neutralizer;

    switch(fs){
        case FightingStyle::Aggressive: {
            if (domain) {
                const bool enough_ce = c->ce_system.cursed_energy >= SorcerySystem::ApplySpendingMultiplier(domain->cost, chr);
                if (!domain->is_active && enough_ce){
                    if (domain_users == 1 || get_random<std::uint8_t>(1, 100) >= 60) {
                        domain->is_active = true;
                    }
                }else if (domain->is_active && !enough_ce && get_random<std::uint8_t>(1, 25) >= 15){
                    domain->is_active = false;
                }
            }
            if (neutral) {
                const bool enough_ce = c->ce_system.cursed_energy >= SorcerySystem::ApplySpendingMultiplier(neutral->cost, chr);
                if (enough_ce){
                    if ((domain && !domain->is_active &&  domain_users == 1) || domain_users == 1) {
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
                const bool enough_ce = c->ce_system.cursed_energy >= SorcerySystem::ApplySpendingMultiplier(domain->cost, chr);
                if (!domain->is_active && enough_ce){
                    if (domain_users == 1 || get_random<std::uint8_t>(1, 100) >= 60) {
                        domain->is_active = true;
                    }
                }else if (domain->is_active && !enough_ce && get_random<std::uint8_t>(1, 25) >= 3){
                    domain->is_active = false;
                }
            }
            if (neutral) {
                const bool enough_ce = c->ce_system.cursed_energy >= SorcerySystem::ApplySpendingMultiplier(neutral->cost, chr);
                if (enough_ce){
                    if ((domain && !domain->is_active &&  domain_users == 1) || domain_users == 1) {
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
                const bool enough_ce = c->ce_system.cursed_energy >= SorcerySystem::ApplySpendingMultiplier(domain->cost, chr);
                if (enough_ce){
                    if (!domain->is_active && (domain_users == 1  || get_random<std::uint8_t>(1, 50) >= 25)) {
                        domain->is_active = true;
                    }
                }else if (domain->is_active && !enough_ce && get_random<std::uint8_t>(1, 25) >= 9){
                    domain->is_active = false;
                }
            }
            if (neutral) {
                const bool enough_ce = c->ce_system.cursed_energy >= SorcerySystem::ApplySpendingMultiplier(neutral->cost, chr);
                if (enough_ce){
                    if ((domain && !domain->is_active &&  domain_users == 1) || domain_users == 1) {
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

void AI::DF_UseTechnique(Character &chr, std::variant<Character*, Shikigami*> target, const Base::DecisionMaker &iq) {
    auto& c = chr.curseUser;

    auto& tech = c->jujutsu.technique;
    if (!tech)return;
    else if (tech->abilities.empty()) return;

    auto* ab = TechniqueSystem::GetAbility(*tech, static_cast<TechniqueSystem::AbilityType>(get_random<std::uint8_t>(0, 3)));
    
    if (!(c->ce_system.cursed_energy >= SorcerySystem::ApplySpendingMultiplier(ab->cost, chr))){
        return;
    }

    int chant_percentage{};
    switch(iq.fighting_style){
        case FightingStyle::Aggressive:{
            chant_percentage = 33;
            break;
        }
        case FightingStyle::Defensive:{
            chant_percentage = 67;
            break;
        }
        case FightingStyle::Mixed:{
            chant_percentage = get_random<int>(1, 100);
            break;
        }
    }
    while(get_random<int>(1, 100) >= chant_percentage && ab->chant_amount < 5) {
        TechniqueSystem::ChantAbility(*ab, chr);
    }

    Log::TechniqueAttack(CombatSystem::ResolveTechnique(chr, *ab, target));
}
void AI::DRM_UseBindingVow(Character &chr, const std::variant<Character*, Shikigami*> tr, const Battlefield &bf, const ResourceUsage &iq) {
    auto& c = chr.curseUser;
    auto& vows = c->jujutsu.binding_vows;
    size_t size{0};
    switch(iq){
        case ResourceUsage::AllOut:
            size = 2;
            break;
        default: [[fallthrough]];
        case ResourceUsage::Conservative:
            size = 0;
            break;
        case ResourceUsage::Mixed:
            size = 1;
            break;
    }
    if (vows.size() < size){
        vows.push_back(VowSystem::CreateVow(chr, chr.brain));  // these vows have insane values, i have no idea what to change except for the percentage values, they just make insane vows on the first turn and kill themselves
    }
}
void AI::DRM_UseShikigami(Character &chr, const ResourceUsage &rs){
    auto& c = chr.curseUser;
    if (c->jujutsu.shikigami.empty()) return;
    SummonType type{};
    switch(rs){
        default:
        case ResourceUsage::AllOut: {     
            if (SorcerySystem::CEMoreThanMax(chr, 0.45)){
                type = SummonType::Active;  
            }else if (SorcerySystem::CEMoreThanMax(chr, 0.15)){
                type = SummonType::Support;
            }else{
                type = SummonType::Shadow;
            }
            break;
        }
        case ResourceUsage::Mixed: {
            if (SorcerySystem::CEMoreThanMax(chr, get_random<double>(0.30, 0.60))){
                type = SummonType::Active;
            }else if (SorcerySystem::CEMoreThanMax(chr, get_random<double>(0.05, 0.30))){
                type = SummonType::Support;
            }else{
                type = SummonType::Shadow;
            }
            break;
        }
        case ResourceUsage::Conservative: {
            if (SorcerySystem::CEMoreThanMax(chr, 0.65)){
                type = SummonType::Active;  
            }else if (SorcerySystem::CEMoreThanMax(chr, 0.40)){
                type = SummonType::Support;
            }else{
                type = SummonType::Shadow;
            }
            break;
        }
    }
    for(auto& s : c->jujutsu.shikigami){
        s.summon_type = type;
    }
}
void AI::DRM_UseReinforcement(Character &c, const ResourceUsage &rs) {
    double val{0.0};
    switch(rs){
        case ResourceUsage::AllOut:
            if (SorcerySystem::CEMoreThanMax(c, 0.40) && !CharacterSystem::HPMoreThanMax(c, 0.40)){
                val = 135.0;
            }else if (SorcerySystem::CEMoreThanMax(c, 0.20) && !CharacterSystem::HPMoreThanMax(c, 0.33)){
                val = 90.0;
            }else if (!CharacterSystem::HPMoreThanMax(c, 0.25)){
                val = 60.0;
            }
            break;
        case ResourceUsage::Mixed:
            if (SorcerySystem::CEMoreThanMax(c, get_random<double>(0.25, 0.75)) && !CharacterSystem::HPMoreThanMax(c, get_random<double>(0.60, 0.90))){
                val = get_random<double>(70.0, 150.0);
            }else if (SorcerySystem::CEMoreThanMax(c, 0.20) && !CharacterSystem::HPMoreThanMax(c, get_random<double>(0.30, 0.60))){
                val = get_random<double>(65.0, 100.0);
            }else if (!CharacterSystem::HPMoreThanMax(c, get_random<double>(0.01, 0.30))){
                val = get_random<double>(20.0, 80.0);
            }
            break;
        case ResourceUsage::Conservative:
            if (SorcerySystem::CEMoreThanMax(c, 0.60) && !CharacterSystem::HPMoreThanMax(c, 0.55)){
                val = 90.0;
            }else if (SorcerySystem::CEMoreThanMax(c, 0.40) && !CharacterSystem::HPMoreThanMax(c, 0.35)){
                val = 50.0;
            }else if (!CharacterSystem::HPMoreThanMax(c, 0.30)){
                val = 35.0;
            }
            break;
    }
    c.curseUser->ce_system.reinforcement_amount = val;
}
void AI::DRM_UseRCT(Character &c, const ResourceUsage &rs) {
    if (!c.curseUser->rct_system.can_use_rct){
        return;
    }
    double val{0.0};
    switch(rs){
        case ResourceUsage::AllOut:
            if (SorcerySystem::CEMoreThanMax(c, 0.40) && !CharacterSystem::HPMoreThanMax(c, 0.40)){
                val = 115.0;
            }else if (SorcerySystem::CEMoreThanMax(c, 0.20) && !CharacterSystem::HPMoreThanMax(c, 0.33)){
                val = 85.0;
            }else if (!CharacterSystem::HPMoreThanMax(c, 0.25)){
                val = 65.0;
            }
            break;
        case ResourceUsage::Mixed:
            if (SorcerySystem::CEMoreThanMax(c, get_random<double>(0.25, 0.75)) && !CharacterSystem::HPMoreThanMax(c, get_random<double>(0.60, 0.90))){
                val = get_random<double>(50.0, 150.0);
            }else if (SorcerySystem::CEMoreThanMax(c, 0.20) && !CharacterSystem::HPMoreThanMax(c, get_random<double>(0.30, 0.60))){
                val = get_random<double>(35.0, 80.0);
            }else if (!CharacterSystem::HPMoreThanMax(c, get_random<double>(0.01, 0.30))){
                val = get_random<double>(10.0, 50.0);
            }
            break;
        case ResourceUsage::Conservative:
            if (SorcerySystem::CEMoreThanMax(c, 0.60) && !CharacterSystem::HPMoreThanMax(c, 0.55)){
                val = 100.0;
            }else if (SorcerySystem::CEMoreThanMax(c, 0.40) && !CharacterSystem::HPMoreThanMax(c, 0.35)){
                val = 55.0;
            }else if (!CharacterSystem::HPMoreThanMax(c, 0.30)){
                val = 50.0;
            }
            break;
    }
    c.curseUser->rct_system.rct_output = val;
}
void AI::DRM_UseTools(Character &c, const std::variant<Character*, Shikigami*> tar) {
    auto& eq = c.equipment;
    if ((!eq.current_tool && !eq.stored_tool && (!eq.has_access_to_inventory || eq.inventory.empty()))){
        return;
    }
    if (std::holds_alternative<Character*>(tar)) {
        auto& tr = std::get<Character*>(tar);
        if (const auto& cr = tr->curseUser){
            if (const auto& t = cr->jujutsu.technique) {
                if (TechniqueSystem::HasBarrier(*t)){
                    CursedToolSystem::SwitchTo(eq, CursedToolSystem::WeaponChoice::TechniqueBypassing);
                    return;
                }
            }
        }
        if (tr->state.health > c.state.health || CharacterSystem::HPMoreThanMax(c, 0.70)){
            CursedToolSystem::SwitchTo(eq, CursedToolSystem::WeaponChoice::HighestDamage);
        }else if (eq.current_tool && eq.current_tool->damage < c.state.strength){
            CursedToolSystem::SwitchTo(eq, CursedToolSystem::WeaponChoice::Unequip);
        }
    }
}

void AI::Fight(Character &user, Battlefield &bf) {
    if (user.state.is_stunned){
        Log::Line("{} Is stunned and their turn will be skipped!", GetInfo::Name(user.identity));
        return;
    }
    const auto& brain{user.brain};
    try {
        std::variant<Character*, Shikigami*> target = AI::GetTarget(user, brain, bf);
        DoResourceManagement(user, target, bf, brain.resource_usage);
        DoFighting(user, target, bf, brain);
    }catch(const std::out_of_range& oor) {
        Log::Error("Range error: {}", oor.what());
    }catch(const std::runtime_error& re){
        Log::Error("Runtime error: {}", re.what());
    }
}