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
#include <cstdio>
#include <variant>
#include <vector>
#include <format>

void Log::WriteMessage(bool newline, std::string_view sv, std::format_args ags) {
    std::vprint_unicode(stdout, sv, ags);
    if (newline) std::fputc('\n', stdout);
}
void Log::WriteError(std::string_view sv, std::format_args ags) {
    std::vprint_unicode(stderr, sv, ags);
    std::fputc('\n', stderr);
}

void Log::CharacterPresentation(const Character &c){
    Log::Line("[{}]{}", GetInfo::Name(c.Identity()), c.State().is_stunned ? "[Stunned]" : "");
    Log::Line("Health-[\x1b[31m{:.1f}/{:.1f}\x1b[0m] | Durability-[\x1b[35m{:.1f}\x1b[0m] | Strength-[\x1b[32m{:.1f}\x1b[0m]", c.State().health, c.State().max_health, c.State().durability, c.State().strength);
    if (const auto& t = c.Equipment().current_tool) {
        Log::Message("Held Tool: [{}] ", GetInfo::Name(t->identity));
    }
    if (const auto& t = c.Equipment().stored_tool){
        Log::Message("Stored Tool: [{}] ", GetInfo::Name(t->identity));
    }
    if (const auto* crs = c.CanUseSorcery()){
        Log::Line("Cursed Energy-[\x1b[36m{:.1f}/{:.1f}\x1b[0m] | Cursed Energy Output-[{}] ", crs->CursedEnergy().cursed_energy, crs->CursedEnergy().max_cursed_energy, Stringet::OutputCmpStr(crs->Output().current_output, crs->Output().max_output_potential));
        Log::Message("Status: [{}] ", Stringet::OutputStatusStr(crs->Output().status));
        if (const auto& tech = crs->Jujutsu().technique){
            Log::Message("Technique: [{}] ", GetInfo::Name(tech->identity));
        }
        if (const auto& domain = crs->Jujutsu().domain){
            Log::Message("Domain: [{}][{}] ", GetInfo::Name(domain->identity), domain->is_active ? "Active":"Inactive");
        }
    }
    Log::Line();
}

void Log::TechniqueInfo(const Technique &ct, const CTLogType log_type, const LogDetailType info_type){
    const bool log_name      = log_type == CTLogType::Name       || log_type == CTLogType::Both;
    const bool log_abilities = log_type == CTLogType::Abilities  || log_type == CTLogType::Both;

    if (log_name){
        Log::Line("TECHNIQUE: [{}]", GetInfo::Name(ct.identity));
    }
    if (log_abilities){
        Log::Line("ABILITIES");
        size_t i = 0;
        for (const auto& c : ct.abilities){
            Log::Line("{}:[{}] Damage: {:.1f} | Output Cost: {:.1f} | CE Cost: {:.1f}", 
                ++i, GetInfo::Name(c.id), c.damage, c.output, c.cost);
        }
    }
}

void Log::DomainSurehit(const SurehitStruct st, const SurehitHit ht){
    if (ht == SurehitHit::Person){
        Log::Line("{} got hit by {} for {:.1f} damage!", GetInfo::Name(st.c->Identity()), GetInfo::Name(st.dm->identity), st.damage);
    }else{
        Log::Line("{}'s {} got damaged by {} for {:.1f} damage", GetInfo::Name(st.c->Identity()), GetInfo::Name(st.c->CanUseSorcery()->Jujutsu().neutralizer->identity), GetInfo::Name(st.dm->identity), st.damage);
    } //                                                                                                                         oh my goodness
}

void Log::Effects(const std::vector<StatusEffect>& v, const Character& p) {
    for (const auto& s : v){
        Log::Line("{} got affected by the {} effect and {} {:.1f} {}", GetInfo::Name(p.Identity()), GetInfo::Name(s.id), s.effect_type == EffectType::Increase ? "gained" : "lost", s.effect_amount, EffectSystem::GetEffectForTypeStr(s.effect_for_type));
    }
}

void Log::TechniqueAttack(const TechniqueStruct tc){
    if (!tc.enough_ce || !tc.enough_output){
        Log::Line("{} tried to use {} but it couldn't be actived due to insufficient {}", GetInfo::Name(tc.user->Identity() ),GetInfo::Name(tc.used_ability->id), tc.enough_output ? "Cursed Energy" : "Output");
        return;
    }
    if (tc.used_ability){
        if (std::holds_alternative<Character*>(tc.target)) {
            auto& c = std::get<Character*>(tc.target);
            if (tc.blocked) {
                Log::Line("{0} tried to use {2} on {1}. It was blocked by {1}'s Technique", GetInfo::Name(tc.user->Identity()), GetInfo::Name(c->Identity()), GetInfo::Name(tc.used_ability->id));
            }else {
                Log::Line("{} used {} on {}, it dealt {:.1f} damage!", GetInfo::Name(tc.user->Identity()), GetInfo::Name(tc.used_ability->id), GetInfo::Name(c->Identity()), tc.damage);
            }
        }else{
            Log::Line("{} used {} on {}, it dealt {:.1f} damage!", GetInfo::Name(tc.user->Identity()), GetInfo::Name(tc.used_ability->id), GetInfo::Name(std::get<Shikigami*>(tc.target)->id), tc.damage);
        }
    }
}
void Log::TechniqueChant(const CurseUser& cu, const TechAbility& tc){
    switch(get_random<int>(0, 10)){
        case 0: Log::Line("Phase!"); break;
        case 1: Log::Line("Twin Meteors!"); break;
        case 2: Log::Line("Eyes of Wisdom!"); break;
        case 3: Log::Line("Scales of the dragon!"); break;
        case 4: Log::Line("Polarized Light!"); break;
        case 5: Log::Line("Crow and Declaration!"); break;
        case 6: Log::Line("Between front and back!"); break;
        case 7: Log::Line("Nine Ropes!"); break;
        case 8: Log::Line("Twilight!"); break;
        case 9: Log::Line("Paramita!"); break;
        case 10: Log::Line("Repulsion!"); break;
    }
    Log::Line("{}'s damage output increases to: {:.1f}", GetInfo::Name(tc.id), TechniqueSystem::CalculateDamage(tc, cu));
}
void Log::Attack(const std::variant<AttackStruct, ToolStruct> vr, const Character& c1, const std::variant<Character*, Shikigami*> c2) {
    std::string str{};
    if (std::holds_alternative<AttackStruct>(vr)){
        const auto& ats = std::get<AttackStruct>(vr);
        if (ats.damage == -1.0) {
            Log::Line("You cannot attack yourself");
            return;
        }
        if (ats.is_critical){
            str.append("\x1b[38;5;124m[CRITICAL]\x1b[0m");
        }
        if (ats.is_blackflash){
            str.append("\x1b[38;5;9m[BLACKFLASH]\x1b[0m");
        }

        if (std::holds_alternative<Character*>(c2)) {
            const auto& ch = std::get<Character*>(c2);
            str.append(std::format("{} attacked {}!\n", GetInfo::Name(c1.Identity()), GetInfo::Name(ch->Identity())));
            if (ats.blocked){
                str.append(std::format("{}'s attack was blocked by {}'s Technique", GetInfo::Name(c1.Identity()), GetInfo::Name(ch->Identity())));
            }else{
                str.append(std::format("{} took {:.1f} damage!", GetInfo::Name(ch->Identity()), ats.damage));
            }
        }else{
            str.append(std::format("{0} attacked {1}!\n{1} took {2:.1f} damage!", GetInfo::Name(c1.Identity()), GetInfo::Name(std::get<Shikigami*>(c2)->id) ,ats.damage));
        }
    }else if (std::holds_alternative<ToolStruct>(vr)){
        const auto& cts = std::get<ToolStruct>(vr);
        if (cts.dealt_damage == -1.0) {
            Log::Line("You cannot attack yourself");
            return;
        }
        if (std::holds_alternative<Character*>(c2)) {
            str.append(std::format("{0} attacked {2}!\n{2} took {1:.1f} damage!\n", GetInfo::Name(c1.Identity()), cts.dealt_damage, GetInfo::Name(std::get<Character*>(c2)->Identity())));
        }else{
            str.append(std::format("{0} attacked {2}!\n{2} took {1:.1f} damage!\n", GetInfo::Name(c1.Identity()), cts.dealt_damage, GetInfo::Name(std::get<Shikigami*>(c2)->id)));
        }
    }
    Log::Line("{0}", str);
}

void Log::Damage(const DamageStruct dms, const Character &attacked){
    if (dms.attack_blocked) {
        Log::Line("{} took no damage. The attack was negated and blocked", GetInfo::Name(attacked.Identity()));
    } else if (dms.negated_damage == 0.0) {
        Log::Line("{} took {:.1f} damage.", GetInfo::Name(attacked.Identity()), dms.damage);
    } else {
        Log::Line("{} took {:.1f} damage. {:.1f} damage has been negated", GetInfo::Name(attacked.Identity()), dms.damage, dms.negated_damage);
    }
}

void Log::Clash(const DomainClashStruct ds) {
    if (ds.winner == ClashWinner::None) {
        Log::Line("{} and {} are locked in battle with each other, cancelling out the sure-hits.", GetInfo::Name(ds.first->identity), GetInfo::Name(ds.second->identity));
        const double d1d = DomainSystem::GetDomainDamage(*ds.first, *ds.second);
        const double d2d = DomainSystem::GetDomainDamage(*ds.second, *ds.first);
        Log::Line("{} took {:.1f} damage and {} took {:.1f} damage!", GetInfo::Name(ds.first->identity), d2d, GetInfo::Name(ds.second->identity), d1d);
        return;
    }

    if (ds.winner == ClashWinner::Both) {
        Log::Message("Both domains have collapsed due to ");
    }else{
        Log::Message("{} has collapsed due to ", ds.winner == ClashWinner::First ? GetInfo::Name(ds.first->identity) : GetInfo::Name(ds.second->identity));
    }

    switch(ds.win_condition){
        case DomainWinCon::Attrition:
            Log::Line("sustaining too much damage!");
            break;
        case DomainWinCon::Refinement:
            Log::Line("being overwhelmed by the other's refinement!");
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
        Log::Line("No characters have been defeated this turn");
        return;
    }
    for (const auto& m : death_messages) {
        Log::Line("{}", m);
    }
    if (death_messages.size() == bf.battlefield.size()) {
        Log::Line("Everyone has been defeated!");
    }
}

void Log::Clear() {
    Log::Message("\x1b[2J\x1b[H");
}