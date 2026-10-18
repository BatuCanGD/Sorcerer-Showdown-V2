#include "../header/Logger.hpp"
#include "../header/Battlefield.hpp"
#include "../header/CharacterType/Character.hpp"
#include "../header/CharacterType/CurseUser.hpp"
#include "../header/Sorcery/Technique.hpp"
#include "../header/Systems/Mechanics/CombatSystem.hpp"
#include "../header/Systems/Mechanics/DomainSystem.hpp"
#include "../header/Systems/Mechanics/EffectSystem.hpp"
#include "../header/Systems/Mechanics/TechniqueSystem.hpp"
#include "../header/Utilities/Random.hpp"
#include "../header/Systems/Stringet.hpp"

#include <print>
#include <string>
#include <variant>
#include <vector>
#include <format>

void Log::Error(std::string_view s, ErrorType type){
    if (!s.empty()){
        std::println(stderr, "{}", s);
        return;
    }
    switch(type){
        case Log::ErrorType::Amount:
            std::println(stderr, "Invalid Amount!");
            break;
        case Log::ErrorType::Choice:
            std::println(stderr, "Invalid Choice!");
            break;
    }
}
void Log::CharacterPresentation(const Character &c){
    std::println("[{}]{}", GetInfo::Name(c.Identity()), c.State().is_stunned ? "[Stunned]" : "");
    std::println("Health-[\x1b[31m{:.1f}/{:.1f}\x1b[0m] | Durability-[\x1b[35m{:.1f}\x1b[0m] | Strength-[\x1b[32m{:.1f}\x1b[0m]", c.State().health, c.State().max_health, c.State().durability, c.State().strength);
    if (const auto& t = c.Equipment().current_tool) {
        std::print("Held Tool: [{}] ", GetInfo::Name(t->identity));  
    }
    if (const auto& t = c.Equipment().stored_tool){
        std::print("Stored Tool: [{}] ", GetInfo::Name(t->identity));  
    }
    if (const auto* crs = c.CanUseSorcery()){
        std::println("Cursed Energy-[\x1b[36m{:.1f}/{:.1f}\x1b[0m] | Cursed Energy Output-[{}] ", crs->CursedEnergy().cursed_energy, crs->CursedEnergy().max_cursed_energy, Stringet::OutputCmpStr(crs->Output().current_output, crs->Output().max_output_potential));
        std::print("Status: [{}] ", Stringet::OutputStatusStr(crs->Output().status));
        if (const auto& tech = crs->Jujutsu().technique){
            std::print("Technique: [{}] ", GetInfo::Name(tech->identity));
        }
        if (const auto& domain = crs->Jujutsu().domain){
            std::print("Domain: [{}][{}] ", GetInfo::Name(domain->identity), domain->is_active ? "Active":"Inactive");
        }
    }
    std::println();
}

void Log::TechniqueInfo(const Technique &ct, const CTLogType log_type, const LogDetailType info_type){
    const bool log_name      = log_type == CTLogType::Name       || log_type == CTLogType::Both;
    const bool log_abilities = log_type == CTLogType::Abilities  || log_type == CTLogType::Both;

    if (log_name){
        std::println("TECHNIQUE: [{}]", GetInfo::Name(ct.identity));
    }
    if (log_abilities){
        std::println("ABILITIES");
        size_t i = 0;
        for (const auto& c : ct.abilities){
            std::println("{}:[{}] Damage: {:.1f} | Output Cost: {:.1f} | CE Cost: {:.1f}", 
                ++i, GetInfo::Name(c.id), c.damage, c.output, c.cost);
        }
    }
}

void Log::DomainSurehit(const SurehitStruct st, const SurehitHit ht){
    if (ht == SurehitHit::Person){
        std::println("{} got hit by {} for {:.1f} damage!", GetInfo::Name(st.c->Identity()), GetInfo::Name(st.dm->identity), st.damage);
    }else{
        std::println("{}'s {} got damaged by {} for {:.1f} damage", GetInfo::Name(st.c->Identity()), GetInfo::Name(st.c->CanUseSorcery()->Jujutsu().neutralizer->identity), GetInfo::Name(st.dm->identity), st.damage);
    } //                                                                                                                         oh my goodness
}

void Log::Effects(const std::vector<StatusEffect>& v, const Character& p) {
    for (const auto& s : v){
        std::println("{} got affected by the {} effect and {} {:.1f} {}", GetInfo::Name(p.Identity()), GetInfo::Name(s.id), s.effect_type == EffectType::Increase ? "gained" : "lost", s.effect_amount, EffectSystem::GetEffectForTypeStr(s.effect_for_type));
    }
}

void Log::TechniqueAttack(const TechniqueStruct tc){
    if (!tc.enough_ce || !tc.enough_output){
        std::println("{} tried to use {} but it couldn't be actived due to insufficient {}", GetInfo::Name(tc.user->Identity() ),GetInfo::Name(tc.used_ability->id), tc.enough_output ? "Cursed Energy" : "Output");
        return;
    }
    if (tc.used_ability){
        if (tc.blocked) {
            std::println("{0} tried to use {2} on {1}. It was blocked by {1}'s Technique", GetInfo::Name(tc.user->Identity()), GetInfo::Name(tc.target->Identity()), GetInfo::Name(tc.used_ability->id));
        }else {
            std::println("{} used {} on {}, it dealt {:.1f} damage!", GetInfo::Name(tc.user->Identity()), GetInfo::Name(tc.used_ability->id), GetInfo::Name(tc.target->Identity()), tc.damage);
        }
    }
}
void Log::TechniqueChant(const CurseUser& cu, const TechAbility& tc){
    switch(get_random<int>(0, 10)){
        case 0: std::println("Phase!"); break;
        case 1: std::println("Twin Meteors!"); break;
        case 2: std::println("Eyes of Wisdom!"); break;
        case 3: std::println("Scales of the dragon!"); break;
        case 4: std::println("Polarized Light!"); break;
        case 5: std::println("Crow and Declaration!"); break;
        case 6: std::println("Between front and back!"); break;
        case 7: std::println("Nine Ropes!"); break;
        case 8: std::println("Twilight!"); break;
        case 9: std::println("Paramita!"); break;
        case 10: std::println("Repulsion!"); break;
    }
    std::println("{}'s damage output increases to: {:.1f}", GetInfo::Name(tc.id), TechniqueSystem::CalculateDamage(tc, cu));
}
void Log::Attack(const std::variant<AttackStruct, ToolStruct> vr, const Character& c1, const Character& c2) {
    std::string str{};
    if (std::holds_alternative<AttackStruct>(vr)){
        const auto& ats = std::get<AttackStruct>(vr);
        if (ats.damage == -1.0) {
            std::println("You cannot attack yourself");
            return;
        }
        std::string info = std::format("{} attacked {}!\n", GetInfo::Name(c1.Identity()), GetInfo::Name(c2.Identity()));
        if (ats.blocked){
            info.append(std::format("{}'s attack was blocked by {}'s Technique", GetInfo::Name(c1.Identity()), GetInfo::Name(c2.Identity())));
        }else{
            info.append(std::format("{} took {:.1f} damage!", GetInfo::Name(c2.Identity()), ats.damage));
            if (ats.is_critical){
                str.append("\x1b[38;5;124m[CRITICAL]\x1b[0m");
            }
            if (ats.is_blackflash){
                str.append("\x1b[38;5;9m[BLACKFLASH]\x1b[0m");
            }
        }
        str.append(info);
    }else if (std::holds_alternative<ToolStruct>(vr)){
        const auto& cts = std::get<ToolStruct>(vr);
        if (cts.dealt_damage == -1.0) {
            std::println("You cannot attack yourself");
            return;
        }
        str.append(std::format("{0} attacked {2}!\n{2} took {1:.1f} damage!\n", GetInfo::Name(c1.Identity()), cts.dealt_damage, GetInfo::Name(c2.Identity())));
    }
    std::println("{0}", str);
}

void Log::Damage(const DamageStruct dms, const Character &attacked){
    if (dms.attack_blocked) {
        std::println("{} took no damage. The attack was negated and blocked", GetInfo::Name(attacked.Identity()));
    } else if (dms.negated_damage == 0.0) {
        std::println("{} took {:.1f} damage.", GetInfo::Name(attacked.Identity()), dms.damage);
    } else {
        std::println("{} took {:.1f} damage. {:.1f} damage has been negated", GetInfo::Name(attacked.Identity()), dms.damage, dms.negated_damage);
    }
}

void Log::Clash(const DomainClashStruct ds) {
    if (ds.winner == ClashWinner::None) {
        std::println("{} and {} are locked in battle with each other, cancelling out the sure-hits.", GetInfo::Name(ds.first->identity), GetInfo::Name(ds.second->identity));
        std::println("{0} took {3:.1f} damage and {1} took {2:.1f} damage!", GetInfo::Name(ds.first->identity), GetInfo::Name(ds.second->identity), ds.f_damage, ds.s_damage);
        return;
    }

    if (ds.winner == ClashWinner::Both) {
        std::print("Both domains have collapsed due to ");
    }else{
        std::print("{} has collapsed due to ", ds.winner == ClashWinner::First ? GetInfo::Name(ds.first->identity) : GetInfo::Name(ds.second->identity));
    }

    switch(ds.win_condition){
        case DomainWinCon::Attrition:
            std::println("sustaining too much damage!");
            break;
        case DomainWinCon::Refinement:
            std::println("being overwhelmed by the other's refinement!");
            break;
        default: 
            break;
    }
}

void Log::Death(const Battlefield& bf){
    std::vector<std::string> death_messages;
    for (const auto& c : bf.battlefield){
        const double hp = c->State().health;
        if (hp > 0.0) continue;
        std::string msg{""}, severity{"DEATH"}, color{"\x1b[38;5;237m"};
        
        if (hp <= -500.0){
            severity = "EXTREME";
            color = "\x1b[38;5;124m";
        }else if (hp <= -250.0){
            severity = "BRUTAL";
            color = "\x1b[38;5;160m";
        }else if (hp <= -100.0){
            severity = "OVERKILL";
            color = "\x1b[38;5;9m";
        }

        msg.append(std::format("[{0}{1}\x1b[0m] ({0}{2:.1f}\x1b[0m)", color, severity, hp));
        msg.append(std::format(" {} has been defeated", GetInfo::Name(c->Identity())));
        death_messages.push_back(msg);
    }
    if (death_messages.empty()){
        std::println("No characters have been defeated this turn");
        return;
    }
    for (const auto& m : death_messages) {
        std::println("{}", m);
    }
    if (death_messages.size() == bf.battlefield.size()) {
        std::println("Everyone has been defeated!");
    }
}