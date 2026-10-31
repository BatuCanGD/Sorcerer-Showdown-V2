#include "../../header/Systems/PlayerSystem.hpp"
#include "../../header/Utilities/Input.hpp"
#include "../../header/Battlefield.hpp"
#include "../../header/Logger.hpp"
#include "../../header/Systems/CharacterSystem.hpp"
#include "../../header/Systems/Mechanics/CombatSystem.hpp"
#include "../../header/Systems/Mechanics/TechniqueSystem.hpp"
#include "../../header/Systems/Mechanics/VowSystem.hpp"
#include "../../header/Systems/Mechanics/SorcerySystem.hpp"
#include "../../header/Logger.hpp"
#include "../../header/Character.hpp"

#include <string_view>
#include <optional>
#include <format>

std::pair<std::vector<Action>, std::vector<std::string>> UserControl::GetChoices(const Character& c) {
    std::vector<Action> ac;
    std::vector<std::string> sc;
    int t{0};
    auto add_ch = [&](std::string_view sv, Action a) {
        sc.push_back(std::format("{}:{}", ++t, sv));
        ac.push_back(a);
    };

    add_ch("Attack", Action::Attack);

    if (c.equipment.current_tool || c.equipment.stored_tool || (c.equipment.has_access_to_inventory && !c.equipment.inventory.empty())) {
        add_ch("Inventory", Action::Inventory);
    }
    if (!c.state.status_effects.empty()){
        add_ch("Status Effects", Action::Effects);
    }
    if (const auto& cr = c.curseUser){
        if (cr->jujutsu.technique) {
            add_ch("Technique", Action::Technique);
        }
        if (cr->jujutsu.domain) {
            add_ch("Domain", Action::Domain);
        }
        if (!cr->jujutsu.shikigami.empty()) {
            add_ch("Shikigami", Action::Shikigami);
        }
        add_ch("Sorcery", Action::Sorcery);
    }
    return {ac, sc};
}

void UserControl::GetPlayerTurn(Character& c, Battlefield& bf) {
    if (c.state.is_stunned){
        return;
    }
    const auto choices = UserControl::GetChoices(c);
    std::uint8_t turn_count = 3;
    do{
        Log::Line("Action count: {}", turn_count);
        for (const auto& str : choices.second){
            Log::Line("{}", str);
        }
        size_t idx = get_input<size_t>() - 1;
        while (idx >= choices.first.size()) {
            Log::Error("Invalid Choice");
            idx = get_input<size_t>() - 1;
        }
        if (UserControl::GetConfirmation()){
            switch(choices.first[idx]){
                case Action::Attack:
                    UserControl::DoAttack(c, UserControl::GetTarget(c, bf));
                    break;
                case Action::Inventory:
                    UserControl::DoInventoryManagement(c);
                    break;
                case Action::Technique:
                    UserControl::DoTechnique(c, bf);
                    break;
                case Action::Domain:
                    UserControl::DoDomain(c);
                    break;
                case Action::Shikigami:
                    UserControl::DoShikigami(c);
                    break;
                case Action::Sorcery:
                    UserControl::DoSorcery(c);
                    break;
                case Action::Effects:
                    UserControl::DoEffects(c);
                    break;
                default: 
                    break;
            }
            turn_count--;
        }

    } while (turn_count > 0);
}

bool UserControl::GetConfirmation() {
    Log::Line("Are you sure?\n 1 - Yes | 2 - No");
    return get_input<int>() == 1;
}
#include <variant>
std::variant<Character*, Shikigami*> UserControl::GetTarget(const Character& cx, Battlefield &bf) {
    std::vector<std::variant<Character*, Shikigami*>> targets;
    targets.push_back(GetCharacterTarget(cx, bf));
    Character* chr = std::get<Character*>(targets.front());
    if (const auto& ch = std::get<Character*>(targets[0])){
        if (auto& crs = ch->curseUser){
            for (auto& c : crs->jujutsu.shikigami) {
                if (c.summon_type == SummonType::Active && c.stats.health > 0.0){
                    targets.emplace_back(&c);
                }
            }
        }
    }
    if (targets.size() == 1) {
        return chr;
    }

    size_t idx{};
    for (const auto& c : targets){
        if (std::holds_alternative<Shikigami*>(c)) {
            auto& sh = std::get<Shikigami*>(c);
            Log::Line("{}:{} | HP:[{:.1f}]", ++idx, GetInfo::Name(sh->id), sh->stats.health);
        }else{
            auto& ch = std::get<Character*>(c);
            Log::Line("{}:{} | HP:[{:.1f}]", ++idx, GetInfo::Name(ch->identity), ch->state.health);
        }
    }
    Log::Line("Choose your target");
    idx = get_input<size_t>();
    if (idx > targets.size()) {
        Log::Error("Invalid Choice");
        return chr;
    }else if (idx > 1){
        return targets[--idx];
    }else{
        return chr;
    }
}

Character* UserControl::GetCharacterTarget(const Character& cx, Battlefield& bf) {
    size_t z{0};
    for (const auto& c : bf.battlefield){
        std::string str{};
        str.append(std::format("{}:{}", ++z, GetInfo::Name(c->identity)));
        str.append(std::format(" {} ", &cx == c.get() ? "(Chosen Character)" : ""));
        str.append(std::format("| HP:[{:.1f}] ", c->state.health));
        if (const auto& cr = c->curseUser){
            auto& j = cr->jujutsu;
            if (!j.shikigami.empty()){
                bool appended{false};
                for (const auto& s : j.shikigami){
                    if (appended) break;
                    if (s.summon_type == SummonType::Active) {
                        str.append("*");
                        appended = true;
                    }
                }
            }
        }
        Log::Line("{}", str);
    }
    Log::Line("Pick a target");
    size_t x = get_input<size_t>() - 1;
    while (x >= bf.battlefield.size()){
        Log::Line("Try again");
        x = get_input<size_t>() - 1;
    } 
    return bf.battlefield[x].get();
}

void UserControl::DoEffects(const Character &cc){
    for (const auto& c : cc.state.status_effects){
        Log::Line("{}", GetInfo::Styalized(c.id));
    }
}

void UserControl::DoAttack(Character& c, std::variant<Character*, Shikigami*> cd) {
    const auto result = CharacterSystem::Attack(c, cd);
    Log::Attack(result, c, cd);
}

void UserControl::DoInventoryManagement(Character& c) {
    auto& equip = c.equipment;
    const bool has_inv = equip.has_access_to_inventory;

    enum class Origin : std::uint8_t { 
        MainHand,
        OffHand,
        Inventory
    };
    struct Candidate final { 
        CursedTool* tool;
        Origin origin;
        size_t index;
    };

    std::vector<Candidate> wp;
    size_t ww{0};

    auto add_wp([&](CursedTool& w, Origin origin, size_t index = 0){
        Log::Line("{}:{}", ++ww, GetInfo::Name(w.identity));
        wp.push_back({&w, origin, index});
    });

    if (auto& current = equip.current_tool) {
        add_wp(*current, Origin::MainHand);
    }
    if (auto& stored  = equip.stored_tool) {
        add_wp(*stored, Origin::OffHand);
    }
    if (has_inv) {
        for (size_t i = 0; i < equip.inventory.size(); ++i) {
            add_wp(equip.inventory[i], Origin::Inventory, i);
        }
    }

    if (wp.empty()) {
        Log::Line("You have no cursed tools"); 
        return;
    }

    size_t input = get_input<size_t>() - 1;
    if (input >= wp.size()) {
        Log::Error("Invalid Choice");
        return;
    }

    const Candidate chosen = wp[input];
    Log::Line("Chosen Tool: [{}]", GetInfo::Name(chosen.tool->identity));
    Log::Line("1 - Move to main hand\n2 - Move to Offhand\n{}", has_inv ? "3 - Move to Inventory" : "");

    auto extract_chosen = [&]() -> CursedTool {
        CursedTool value = std::move(*chosen.tool);
        switch (chosen.origin) {
            case Origin::MainHand:
                equip.current_tool.reset();
                break;
            case Origin::OffHand:
                equip.stored_tool.reset();
                break;
            case Origin::Inventory:
                equip.inventory.erase(equip.inventory.begin() + chosen.index);
                break;
        }
        return value;
    };

    switch (get_input<int>()) {
        case 1: {
            if (chosen.origin == Origin::MainHand) {
                Log::Line("You cannot move an item in your hand");
                return;
            }
            CursedTool incoming = extract_chosen();
            if (equip.current_tool) {
                CursedTool displaced = std::move(*equip.current_tool);
                if (has_inv) {
                    equip.inventory.push_back(std::move(displaced));
                } else {
                    equip.stored_tool = std::move(displaced);
                }
            }
            equip.current_tool = std::move(incoming);
            break;
        }
        case 2: {
            if (chosen.origin == Origin::OffHand) {
                Log::Line("You cannot move an item in your hand");
                return;
            }
            CursedTool incoming = extract_chosen();
            if (equip.stored_tool) {
                CursedTool displaced = std::move(*equip.stored_tool);
                if (has_inv) {
                    equip.inventory.push_back(std::move(displaced));
                } else {
                    equip.current_tool = std::move(displaced);
                }
            }
            equip.stored_tool = std::move(incoming);
            break;
        }
        case 3: {
            if (!has_inv || chosen.origin == Origin::Inventory) {
                return;
            }
            equip.inventory.push_back(extract_chosen());
            break;
        }
        default:
            break;
    }
}

void UserControl::DoSorcery(Character& c) {
    if (!c.curseUser.has_value()){
        Log::Line("You are not a curse user!");
        return;
    }
    const auto& sp = UserControl::GetSorceryChoices(c);
    size_t ch = get_input<size_t>() - 1;

    if (ch >= sp.size()) {
        return;
    }

    switch(sp[ch]){
        case SorceryType::RCT:
            UserControl::ForRCT(c);
            break;
        case SorceryType::Reinforcement:
            UserControl::ForReinforcement(c);
            break;
        case SorceryType::BindingVows:
            UserControl::ForBindingVows(c);
            break;
        case SorceryType::Amplification:
            UserControl::ForAmplification(c);
            break;
        default:
            Log::Error("Cannot find SorceryType type");
    }
}

const std::vector<UserControl::SorceryType> UserControl::GetSorceryChoices(const Character& c) {
    std::vector<SorceryType> sp;
    int k{0};
    auto add_p([&](std::string_view sv, SorceryType s){
        Log::Line("{} - {}", ++k, sv);
        sp.push_back(s);
    });
    auto& cr = c.curseUser;
    if (cr->rct_system.can_use_rct){
        add_p("RCT", SorceryType::RCT);
    }
    if (cr->amplification.can_use_amp){
        add_p("Amplification", SorceryType::Amplification);
    }
    add_p("Reinforcement", SorceryType::Reinforcement);
    add_p("Binding Vows", SorceryType::BindingVows);
    return sp;
}

void UserControl::ForAmplification(Character &c){
    bool& amp = c.curseUser->amplification.is_active;
    Log::Line("Amplification Status: [{}]", amp ? "Active" : "Inactive");
    Log::Line("1 - Enable | 2 - Disable | 3 - Return");
    int g = get_input<int>();
    switch(g){
        case 1:
            amp = true;
            break;
        case 2:
            amp = false;
            break;
        case 3:
            return;
        default:
            Log::Error("Invalid Input");
    }

}
void UserControl::ForRCT(Character& c){
    double& rct = c.curseUser->rct_system.rct_output;
    Log::Line("Total RCT output: {:.1f} | Cost: {:.1f} CE per turn", rct, SorcerySystem::ApplyRCTCost(rct));
    Log::Line("1 - Set | 2 - Return");
    if (get_input<int>() != 1) return;
    Log::Line("Enter Output Amount: ");
    const double prv = rct;
    rct = std::max(get_input<double>(), 0.0);

    auto& cr = c.curseUser;
    const double new_cost = SorcerySystem::ApplyRCTCost(rct);
    if (new_cost > cr->ce_system.cursed_energy){
        Log::Line("New cost: Will consume all of your Cursed Energy");
    }else{
        Log::Line("New cost: {:.1f} CE per turn", new_cost);
    }
    if (!UserControl::GetConfirmation()){
        rct = prv;
    }
}
void UserControl::ForReinforcement(Character& c){
    double& rf = c.curseUser->ce_system.reinforcement_amount;
    Log::Line("Total Reinforcement Usage: {:.1f} | Cost: {:.1f} CE per turn", rf, SorcerySystem::ApplyReinforcementCost(rf));
    Log::Line("1 - Set | 2 - Return");
    if (get_input<int>() != 1) return;
    Log::Line("Enter Reinforcement Amount: ");
    const double prv = rf;
    rf = std::max(get_input<double>(), 0.0);

    auto& cr = c.curseUser;
    const double new_cost = SorcerySystem::ApplyReinforcementCost(rf);
    if (new_cost > cr->ce_system.cursed_energy){
        Log::Line("New cost: Will consume all of your Cursed Energy");
    }else{
        Log::Line("New cost: {:.1f} CE per turn", new_cost);
    }
    if (!UserControl::GetConfirmation()){
        rf = prv;
    }
}
void UserControl::ForBindingVows(Character& c){ 
    Log::Line("Binding vows can only be created and are not able to be removed!");
    Log::Line("Current Binding Vows: ");
    auto& cr = c.curseUser;
    if (cr->jujutsu.binding_vows.empty()) {
        Log::Line("None");
    }
    for (const auto& cc : cr->jujutsu.binding_vows){
        Log::Line("[{}]-{}", GetInfo::Name(cc.identity), cc.identity.description);
    }
    Log::Line("1 - Create Binding Vow | 2 - Return");

    if (get_input<int>() != 1) return;
    BindingVow bv{};

    Log::Line("Which will you sacrifice?");

    std::vector<SacrificeType> stv;
    auto opt = [&](std::string_view s, SacrificeType st){
        stv.push_back(st);
        Log::Line("{}:{}", stv.size(), s);
    };
    opt("Health", SacrificeType::Health);
    opt("Cursed Energy", SacrificeType::CursedEnergy);
    opt("Cursed Energy Output", SacrificeType::OutputPotential);

    const size_t sac = get_input<size_t>() - 1;
    if (sac >= stv.size()){
        return;
    }
    const auto sack = stv[sac];
    stv.clear();
    Log::Line("Which will you gain?");

    opt("Health", SacrificeType::Health);
    opt("Cursed Energy", SacrificeType::CursedEnergy);
    opt("Cursed Energy Output", SacrificeType::OutputPotential);

    const size_t gan = get_input<size_t>() - 1;
    if (gan >= stv.size()) return;
    const auto gain = stv[gan];
    if (gain == sack) {
        Log::Line("You cannot sacrifice and gain the same thing");
        return;
    }
    Log::Line("How much will you sacrifice percentage-wise?\n[1-99]\n=> ");
    double dp = get_input<double>();
    if (dp < 0.01) {
        Log::Line("You must sacrifice at least 1%");
        return;
    } else if (dp > 99.9 || (dp > 0.99 && dp < 1.0)){
        Log::Line("You cannot sacrifice more than 99%");
        return;
    }
    const double sdp = (dp >= 1.0 && dp <= 99.9) ? dp * 0.01 : dp;

    bv.gain = gain;
    bv.loss = sack;
    bv.percentage = sdp;
    bv.identity = VowSystem::GenerateVowId(bv);
    cr->jujutsu.binding_vows.push_back(bv);
}

void UserControl::DoTechnique(Character& c, Battlefield& bf) {
    if (!c.curseUser.has_value()){
        Log::Line("You are not a curse user!");
        return;
    }
    auto& cr = c.curseUser;
    if (!cr->jujutsu.technique.has_value()) {
        Log::Line("You do not have a technique");
        return;
    }
    auto& tc = cr->jujutsu.technique;
    
    if (tc->barrier.can_use_barrier){
        Log::TechniqueInfo(*tc, Log::CTLogType::Name);
        Log::Line("[{}]", tc->barrier.is_active ? "Barrier Active" : "Barrier Inactive");

        Log::Line("1 - Set Barrier | 2 - Use Ability | 3 - Return");
        int ch = get_input<int>();
        if (ch == 1){
            Log::Line("1 - Activate | 2 - Deactivate");
            TechniqueSystem::SetBarrier(*cr, get_input<int>() == 1);
        }else if (ch == 3){
            return;
        }
    }

    auto& ability = TechniqueSystem::ChooseAbility(*tc);
    Log::Line("1 - Use Ability | 2 - Chant");
    if (get_input<int>() == 1){
        auto cd = UserControl::GetTarget(c, bf);
        const auto result = CombatSystem::ResolveTechnique(c, ability, cd);
        Log::TechniqueAttack(result);
    }else{
        TechniqueSystem::ChantAbility(ability, c);
    }
}
void UserControl::DoDomain(Character& c) {
    if (!c.curseUser.has_value()){
        Log::Line("You are not a curse user!");
        return;
    }
    auto& cr = c.curseUser;
    auto& domain = cr->jujutsu.domain;
    auto& neutralizer = cr->jujutsu.neutralizer;

    if (!domain && !neutralizer) {
        Log::Line("You do not have a domain or anything that can neutralize it");
        return;
    }
    enum class dmn : std::uint8_t { Domain, Neutralizer };
    std::vector<dmn> dm;
    unsigned short a = 0;
    auto add_ch = [&](dmn s){ 
        dm.push_back(s);
        Log::Line("{}:{} ", ++a, s == dmn::Domain ? "Domain" : "Neutralizer");
    };
    if (domain){
        add_ch(dmn::Domain);
        Log::Line("[{}][{:.1f}/{:.1f}] - {}", GetInfo::Name(domain->identity), domain->health, domain->max_health, domain->is_active ? "Active" : "Inactive");
    }
    if (neutralizer) {
        add_ch(dmn::Neutralizer);
        Log::Line("[{}][{:.1f}/{:.1f}] - {}", GetInfo::Name(neutralizer->identity), neutralizer->health, neutralizer->max_health,  neutralizer->is_active ? "Active" : "Inactive"); 
    }

    size_t pl = get_input<size_t>() - 1;
    if (pl >= dm.size()){
        return;
    }
    switch(dm[pl]){
        case dmn::Domain: {
            Log::Line("[{}] - {}", GetInfo::Name(domain->identity), domain->is_active ? "Active" : "Inactive");
            Log::Line("1 - Activate | 2 - Deactivate\n=>");
            int pch = get_input<int>();
            if (pch == 1){
                domain->is_active = true;
            }else if (pch == 2){
                domain->is_active = false;
            }
            break;
        }
        case dmn::Neutralizer: {
            Log::Line("[{}] - {}", GetInfo::Name(neutralizer->identity), neutralizer->is_active ? "Active" : "Inactive"); 
            Log::Line("1 - Activate | 2 - Deactivate\n=>");
            int pch = get_input<int>();
            if (pch == 1){
                neutralizer->is_active = true;
            }else if (pch == 2){
                neutralizer->is_active = false;
            }
            break;
        }
    }
}

//
// Shikigami player Functions
//

void UserControl::DoShikigami(Character& c) {
    if (!c.curseUser.has_value()){
        Log::Line("You are not a curse user!");
        return;
    }
    auto& cr = c.curseUser;
    if (cr->jujutsu.shikigami.empty()) {
        Log::Line("You do not have any shikigami");
        return;
    }
    UserControl::UseShikigami(*UserControl::ChooseShikigami(cr->jujutsu.shikigami));
}

Shikigami* UserControl::ChooseShikigami(std::vector<Shikigami>& sh) {
    size_t idx{0};
    for (const auto& x : sh){
        Log::Line("{}:{}", ++idx, GetInfo::Name(x.id));
    }

    idx = get_input<size_t>() - 1;

    while (idx >= sh.size()) {
        Log::Error("Invalid Choice");
        idx = get_input<size_t>() - 1;
    }

    return &sh[idx];
}
void UserControl::UseShikigami(Shikigami& c) {
    const auto& active_type = c.summon_type;
    std::string active;
    switch(active_type){
        case SummonType::Support:
            active = "Partially Manifested";
            break;
        case SummonType::Active:
            active = "Fully Manifested";
            break;
        case SummonType::Shadow:
            active = "In Shadow";
            break;
    }
    Log::Line("Chosen: {} -  {}\n1 - Manifest\n2 - Partial Manifestation\n3 - Dismiss", GetInfo::Name(c.id), active);
    int g = get_input<int>();
    switch(g) {
        case 1:
            if (active_type == SummonType::Active){
                Log::Line("Shikigami is already fully manifested");
                return;
            }
            c.summon_type = SummonType::Active;
            break;
        case 2:
            if (active_type == SummonType::Support){
                Log::Line("Shikigami is already partially manifested");
                return;
            }
            c.summon_type = SummonType::Support;
            break;
        case 3:
            if (active_type == SummonType::Shadow){
                Log::Line("Shikigami is already dismissed");
                return;
            }
            c.summon_type = SummonType::Shadow;
            break;
        default:
            break;
    }
}