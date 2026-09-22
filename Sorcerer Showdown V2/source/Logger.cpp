#include "../header/Logger.hpp"
#include "../header/Battlefield.hpp"
#include "../header/CharacterType/Character.hpp"
#include "../header/CharacterType/CurseUser.hpp"
#include "../header/Sorcery/Technique.hpp"
#include "../header/Systems/System/CombatSystem.hpp"
#include "../header/Systems/System/DomainSystem.hpp"
#include "../header/Systems/System/EffectSystem.hpp"
#include "../header/Systems/Stringet.hpp"

#include <print>
#include <string>
#include <vector>
#include <format>


void Log::CharacterInfo(const Character &c){
    std::println("[{}]", c.Name());
    std::print("HP: [{}] | DURA: [{}] | STR: [{}]", Stringet::HealthStr(c.Health()), Stringet::DurabilityStr(c.State().durability), Stringet::StrengthStr(c.State().strength));
    if (const auto* crs = c.CanUseSorcery()){
        std::println("CE: [{}] | EFFICIENCY: [{}] ", Stringet::OutputStr(crs->Output().max_output_potential), Stringet::EfficiencyStr(crs->Sorcery().efficiency));
        if (const auto& tech = crs->Jujutsu().technique){
            Log::TechniqueInfo(*tech);
        }
    }
    std::println();
}

void Log::TechniqueInfo(const Technique &ct, const CTLogType& log_type){
    const bool log_name      = log_type == CTLogType::Name       || log_type == CTLogType::Both;
    const bool log_abilities = log_type == CTLogType::Abilities  || log_type == CTLogType::Both;

    if (log_name){
        std::println("TECHNIQUE: [{}]", ct.Name());
    }
    if (log_abilities){
        std::println("ABILITIES");
        size_t i = 0;
        for (const auto& c : ct.Abilities()){
            std::println("{}:[{}{}{}] Damage: {:.1f} | Output Cost: {:.1f} | CE Cost: {:.1f}", 
                ++i, c.id.color, c.id.name, c.id.color.empty() ? "" : "\x1b[0m", c.damage, c.output, c.cost);
        }
    }
}

void Log::Effects(std::pair<const std::vector<StatusEffect>&, const Character&> ef) {
    for (const auto& s : ef.first){
        std::print("{} got affected by the {}{}{} effect and {} {} {}", ef.second.Name(), s.id.color ,s.id.name, s.id.color.empty() ? "" : "\x1b[0m", s.effect_type == EffectType::Increase ? "gained" : "lost", s.effect_amount, EffectSystem::GetEffectForTypeStr(s.effect_for_type));
    }
}


void Log::Attack(const AttackStruct ats, const Character& c1, const Character& c2) {
    const std::string info = std::format("{} took {:.1f} damage from {}!", c1.Name(), ats.damage, c2.Name());
    std::string word{};
    if (ats.is_critical){
        word.append("\x1b[38;5;124m[CRITICAL]\x1b[0m");
    }
    if (ats.is_blackflash){
        word.append("\x1b[38;5;9m[BLACKFLASH]\x1b[0m");
    }
    word.append(info);
    std::println("{}", word);
}

void Log::Damage(const DamageStruct dms, const Character &attacked){
    if (dms.attack_blocked) {
        std::println("{} took no damage. The attack was negated and blocked", attacked.Name());
    } else if (dms.negated_damage == 0.0) {
        std::println("{} took {:.1f} damage.", attacked.Name(), dms.damage);
    } else {
        std::println("{} took {:.1f} damage. {:.1f} damage has been negated", attacked.Name(), dms.damage, dms.negated_damage);
    }
}

void Log::Clash(const ClashWinner winner, const DomainWinCon win_con) {
    if (winner == ClashWinner::None) {
        std::println("The domains are locked in battle with each other, cancelling out the sure-hits.");
        return;
    }
    const bool both_domains = winner == ClashWinner::Both;
    if (both_domains) {
        std::print("Both domains have collapsed due to ");
    }else{
        std::print("A domain has collapsed due to ");
    }

    switch(win_con){
        case DomainWinCon::Attrition:
            std::println("sustaining too much damage!");
            break;
        case DomainWinCon::Refinement:
            std::println("being overwhelmed by the other's refinement!");
            break;
        case DomainWinCon::Overwhelmed:
            std::println("being overwhelmed by the range!");
            break;
        default: 
            break;
    }
}

void Log::Death(const Battlefield& bf){
    std::vector<std::string> death_messages;
    for (const auto& c : bf.battlefield){
        const double hp = c->Health();
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

        msg.append(std::format("[{0}{1}\x1b[0m] ({0}{2}\x1b[0m)", color, severity, hp));
        msg.append(std::format(" {} has been defeated\n", c->Name()));
        death_messages.push_back(msg);
    }
    if (death_messages.empty()){
        std::println("No characters have been defeated this turn");
        return;
    }
    for (const auto& m : death_messages) {
        std::println("{}", m);
    }
}

void Log::d_CharacterInfo(const Character& c){
    std::println("{} | HEALTH: {:.1f} | STRENGTH: {:.1f} | DURABILITY: {:.1f}", c.Name(), c.Health() ,c.State().strength, c.State().durability);
    if (const auto* crs = c.CanUseSorcery()) {
        std::println("CURSED ENERGY: {:.1f} | CE EFFICIENCY: {}", crs->CursedEnergy(), Stringet::EfficiencyStr(crs->Sorcery().efficiency));
        std::println("TECHNIQUE: {}", crs->Jujutsu().technique ? crs->Jujutsu().technique->Name() : "None");
    }
}

void Log::d_TechniqueInfo(const Technique &ct){
    std::println("{}", ct.Name());
    for ([[maybe_unused]] const auto& [id, damage, cost, output, at_type] : ct.Abilities()){
        std::println("[{}{}{}] {:.1f} damage |{:.1f} cursed energy cost |{:.1f} output cost", 
            id.color, id.name, id.color.empty() ? "" : "\x1b[0m", damage, cost, output);
    }
}