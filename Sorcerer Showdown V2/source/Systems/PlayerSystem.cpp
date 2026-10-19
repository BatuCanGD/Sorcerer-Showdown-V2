#include "../../header/Systems/PlayerSystem.hpp"
#include "../../header/Utilities/Input.hpp"
#include "../../header/Battlefield.hpp"
#include "../../header/Systems/Mechanics/CombatSystem.hpp"
#include "../../header/Systems/Mechanics/TechniqueSystem.hpp"
#include "../../header/Systems/Mechanics/VowSystem.hpp"
#include "../../header/Systems/Mechanics/SorcerySystem.hpp"
#include "../../header/Logger.hpp"
#include "../../header/CharacterType/Character.hpp"
#include "../../header/CharacterType/CurseUser.hpp"

#include <string_view>
#include <optional>
#include <format>
#include <print>


std::pair<std::vector<Action>, std::vector<std::string>> UserControl::GetChoices(const Character& c) {
    std::vector<Action> ac;
    std::vector<std::string> sc;
    int t{0};
    auto add_ch = [&](std::string_view sv, Action a) {
        sc.push_back(std::format("{}:{}", ++t, sv));
        ac.push_back(a);
    };

    add_ch("Attack", Action::Attack);

    if (c.Equipment().current_tool || c.Equipment().stored_tool || (c.Equipment().has_access_to_inventory && !c.Equipment().inventory.empty())) {
        add_ch("Inventory", Action::Inventory);
    }

    if (const auto* cr = c.CanUseSorcery()){
        if (cr->Jujutsu().technique) {
            add_ch("Technique", Action::Technique);
        }
        if (cr->Jujutsu().domain) {
            add_ch("Domain", Action::Domain);
        }
        if (!cr->Jujutsu().shikigami.empty()) {
            add_ch("Shikigami", Action::Shikigami);
        }
        add_ch("Sorcery", Action::Sorcery);
    }
    return {ac, sc};
}

void UserControl::GetPlayerTurn(Character& c, Battlefield& bf) {
    const auto choices = UserControl::GetChoices(c);
    auto* crs = c.CanUseSorcery();
    std::uint8_t turn_count = 3;
    do{
        std::println("Action count: {}", turn_count);
        for (const auto& str : choices.second){
            std::println("{}", str);
        }
        size_t idx = get_input<size_t>() - 1;
        while (idx >= choices.first.size()) {
            Log::Error();
            idx = get_input<size_t>() - 1;
        }
        if (UserControl::GetConfirmation()){
            switch(choices.first[idx]){
                case Action::Attack:
                    UserControl::DoAttack(c, UserControl::GetTarget(bf));
                    break;
                case Action::Inventory:
                    UserControl::DoInventoryManagement(c);
                    break;
                case Action::Technique:
                    UserControl::DoTechnique(crs, bf);
                    break;
                case Action::Domain:
                    UserControl::DoDomain(crs);
                    break;
                case Action::Shikigami:
                    UserControl::DoShikigami(crs);
                    break;
                case Action::Sorcery:
                    UserControl::DoSorcery(crs);
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
    std::println("Are you sure?\n 1 - Yes | 2 - No");
    return get_input<int>() == 1;
}
#include <variant>
std::variant<Character*, Shikigami*> UserControl::GetTarget(Battlefield &bf) {
    std::vector<std::variant<Character*, Shikigami*>> targets;
    targets.push_back(GetCharacterTarget(bf));
    Character* chr = std::get<Character*>(targets.front());
    if (const auto& ch = std::get<Character*>(targets[0])){
        if (auto* crs = ch->CanUseSorcery()){
            for (auto c : crs->Jujutsu().shikigami) {
                if (c.summon_type == SummonType::Active){
                    targets.emplace_back(&c);
                }
            }
        }
    }
    if (targets.size() == 1) {
        return chr;
    }

    std::uint8_t idx{};
    for (const auto& c : targets){
        if (std::holds_alternative<Shikigami*>(c)) {
            auto& sh = std::get<Shikigami*>(c);
            std::println("{}:{} | {}", ++idx, GetInfo::Name(sh->id), sh->stats.health);
        }else{
            auto& ch = std::get<Character*>(c);
            std::println("{}:{} | {}", ++idx, GetInfo::Name(ch->Identity()), ch->State().health);
        }
    }
    std::println("Choose your target");
    size_t pc = get_input<size_t>();
    if (pc > targets.size()) {
        Log::Error();
        return chr;
    }else if (pc > 1){
        return targets[--pc];
    }else{
        return chr;
    }
}

Character* UserControl::GetCharacterTarget(Battlefield& bf) {
    size_t z{0};
    for (const auto& c : bf.battlefield){
        std::string str{};
        str.append(std::format("{}:{}", ++z, GetInfo::Name(c->Identity())));
        str.append(" | {} ", c->State().health);
        if (const auto& cr = c->CanUseSorcery()){
            auto& j = cr->Jujutsu();
            if (!j.shikigami.empty()){
                bool appended{false};
                for (const auto& s : j.shikigami){
                    if (appended) continue;
                    if (s.summon_type == SummonType::Active) {
                        str.append("*");
                        appended = true;
                    }
                }
            }
        }
        std::println("{}", str);
    }
    std::println("Pick a target");
    size_t x = get_input<size_t>() - 1;
    while (x >= bf.battlefield.size()){
        std::println("Try again");
        x = get_input<size_t>() - 1;
    } 
    return bf.battlefield[x].get();
}

void UserControl::DoEffects(const Character &cc){
    for (const auto& c : cc.State().status_effects){
        std::println("{}", GetInfo::Styalized(c.id));
    }
}

void UserControl::DoAttack(Character& c, std::variant<Character*, Shikigami*> cd) {
    const auto result = c.Attack(cd);
    Log::Attack(result, c, cd);
}

void UserControl::DoInventoryManagement(Character& c) {
    auto& equip = c.Equipment();
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
        std::println("{}:{}", ++ww, GetInfo::Name(w.identity));
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
        std::println("You have no cursed tools"); 
        return;
    }

    size_t input = get_input<size_t>() - 1;
    if (input >= wp.size()) {
        Log::Error();
        return;
    }

    const Candidate chosen = wp[input];
    std::println("Chosen Tool: [{}]", GetInfo::Name(chosen.tool->identity));
    std::println("1 - Move to main hand\n2 - Move to Offhand\n{}", has_inv ? "3 - Move to Inventory" : "");

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
                std::println("You cannot move an item in your hand");
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
                std::println("You cannot move an item in your hand");
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

void UserControl::DoSorcery(CurseUser* c) {
    if (c == nullptr){
        std::println("You are not a curse user!");
        return;
    }
    const auto& sp = UserControl::GetSorceryChoices(*c);
    size_t ch = get_input<size_t>() - 1;

    if (ch >= sp.size()) {
        return;
    }

    switch(sp[ch]){
        case SorceryType::RCT:
            UserControl::ForRCT(*c);
            break;
        case SorceryType::Reinforcement:
            UserControl::ForReinforcement(*c);
            break;
        case SorceryType::BindingVows:
            UserControl::ForBindingVows(*c);
            break;
    }
}

const std::vector<UserControl::SorceryType> UserControl::GetSorceryChoices(const CurseUser& c) {
    std::vector<SorceryType> sp;
    int k{0};
    auto add_p([&](std::string_view sv, SorceryType s){
        std::println("{} - {}", ++k, sv);
        sp.push_back(s);
    });
    if (c.RCT().can_use_rct){
        add_p("RCT", SorceryType::RCT);
    }
    add_p("Reinforcement", SorceryType::Reinforcement);
    add_p("Binding Vows", SorceryType::BindingVows);
    return sp;
}


void UserControl::ForRCT(CurseUser& c){
    double& rct = c.RCT().rct_output;
    std::println("Total RCT output: {:.1f} | Cost: {:.1f} CE per turn", rct, SorcerySystem::ApplyRCTCost(rct));
    std::println("1 - Set | 2 - Return");
    if (get_input<int>() != 1) return;
    std::print("Enter Output Amount: ");
    const double prv = rct;
    rct = std::max(get_input<double>(), 0.0);

    std::println("New cost: {:.1f} CE per turn", SorcerySystem::ApplyRCTCost(rct));
    if (!UserControl::GetConfirmation()){
        rct = prv;
    }
}
void UserControl::ForReinforcement(CurseUser& c){
    double& rf = c.CursedEnergy().reinforcement_amount;
    std::println("Total Reinforcement Usage: {:.1f} | Cost: {:.1f} CE per turn", rf, SorcerySystem::ApplyReinforcementCost(rf));
    std::println("1 - Set | 2 - Return");
    if (get_input<int>() != 1) return;
    std::print("Enter Reinforcement Amount: ");
    const double prv = rf;
    rf = std::max(get_input<double>(), 0.0);

    std::println("New cost: {:.1f} CE per turn", SorcerySystem::ApplyReinforcementCost(rf));
    if (!UserControl::GetConfirmation()){
        rf = prv;
    }
}
void UserControl::ForBindingVows(CurseUser& c){ 
    std::println("Binding vows can only be created and are not able to be removed!");
    std::println("Current Binding Vows: ");
    if (c.Jujutsu().binding_vows.empty()) {
        std::println("None");
    }
    for (const auto& cc : c.Jujutsu().binding_vows){
        std::println("[{}]-{}", GetInfo::Name(cc.identity), cc.identity.description);
    }
    std::println("1 - Create Binding Vow | 2 - Return");

    if (get_input<int>() != 1) return;
    BindingVow bv{};

    std::println("Which will you sacrifice?");

    std::vector<SacrificeType> stv;
    auto opt = [&](std::string_view s, SacrificeType st){
        stv.push_back(st);
        std::println("{}:{}", stv.size(), s);
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
    std::println("Which will you gain?");

    opt("Health", SacrificeType::Health);
    opt("Cursed Energy", SacrificeType::CursedEnergy);
    opt("Cursed Energy Output", SacrificeType::OutputPotential);

    const size_t gan = get_input<size_t>() - 1;
    if (gan >= stv.size()) return;
    const auto gain = stv[gan];
    if (gain == sack) {
        std::println("You cannot sacrifice and gain the same thing");
        return;
    }
    std::println("How much will you sacrifice percentage-wise?\n[1-99]\n=> ");
    double dp = get_input<double>();
    if (dp < 0.01) {
        std::println("You must sacrifice at least 1%");
        return;
    } else if (dp > 99.9 || (dp > 0.99 && dp < 1.0)){
        std::println("You cannot sacrifice more than 99%");
        return;
    }
    const double sdp = (dp >= 1.0 && dp <= 99.9) ? dp * 0.01 : dp;

    bv.gain = gain;
    bv.loss = sack;
    bv.percentage = sdp;
    bv.identity = VowSystem::GenerateVowId(bv);
    c.Jujutsu().binding_vows.push_back(bv);
}

void UserControl::DoTechnique(CurseUser* c, Battlefield& bf) {
    if (c == nullptr){
        std::println("You are not a curse user!");
        return;
    }
    if (!c->Jujutsu().technique.has_value()) {
        std::println("You do not have a technique");
        return;
    }
    auto& ability = TechniqueSystem::ChooseAbility(*c->Jujutsu().technique);
    std::println("1 - Use Ability | 2 - Chant");
    if (get_input<int>() == 1){
        auto cd = UserControl::GetTarget(bf);
        const auto result = CombatSystem::ResolveTechnique(*c, ability, cd);
        Log::TechniqueAttack(result);
    }else{
        TechniqueSystem::ChantAbility(ability, *c);
    }
}
void UserControl::DoDomain(CurseUser* c) {
    if (c == nullptr){
        std::println("You are not a curse user!");
        return;
    } 
    auto& domain = c->Jujutsu().domain;
    auto& neutralizer = c->Jujutsu().neutralizer;

    if (!domain && !neutralizer) {
        std::println("You do not have a domain or anything that can neutralize it");
        return;
    }
    enum class dmn : std::uint8_t { Domain, Neutralizer };
    std::vector<dmn> dm;
    unsigned short a = 0;
    auto add_ch = [&](dmn s){ 
        dm.push_back(s);
        std::print("{}:{} ", ++a, s == dmn::Domain ? "Domain" : "Neutralizer");
    };
    if (domain){
        add_ch(dmn::Domain);
        std::println("[{}][{:.1f}/{:.1f}] - {}", GetInfo::Name(domain->identity), domain->health, domain->max_health, domain->is_active ? "Active" : "Inactive");
    }
    if (neutralizer) {
        add_ch(dmn::Neutralizer);
        std::println("[{}][{:.1f}/{:.1f}] - {}", GetInfo::Name(neutralizer->identity), neutralizer->health, neutralizer->max_health,  neutralizer->is_active ? "Active" : "Inactive"); 
    }

    size_t pl = get_input<size_t>() - 1;
    if (pl >= dm.size()){
        return;
    }
    switch(dm[pl]){
        case dmn::Domain: {
            std::println("[{}] - {}", GetInfo::Name(domain->identity), domain->is_active ? "Active" : "Inactive");
            std::println("1 - Activate | 2 - Deactivate\n=>");
            int pch = get_input<int>();
            if (pch == 1){
                domain->is_active = true;
            }else if (pch == 2){
                domain->is_active = false;
            }
            break;
        }
        case dmn::Neutralizer: {
            std::println("[{}] - {}", GetInfo::Name(neutralizer->identity), neutralizer->is_active ? "Active" : "Inactive"); 
            std::println("1 - Activate | 2 - Deactivate\n=>");
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

void UserControl::DoShikigami(CurseUser* c) {
    if (c == nullptr){
        std::println("You are not a curse user!");
        return;
    }
    if (c->Jujutsu().shikigami.empty()) {
        std::println("You do not have any shikigami");
        return;
    }
    UserControl::UseShikigami(*UserControl::ChooseShikigami(c->Jujutsu().shikigami));
}

Shikigami* UserControl::ChooseShikigami(std::vector<Shikigami>& sh) {
    size_t idx{0};
    for (const auto& x : sh){
        std::println("{}:{}", ++idx, GetInfo::Name(x.id));
    }

    idx = get_input<size_t>() - 1;

    while (idx >= sh.size()) {
        Log::Error();
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
    std::println("Chosen: {} -  {}\n1 - Manifest\n2 - Partial Manifestation\n3 - Dismiss", GetInfo::Name(c.id), active);
    int g = get_input<int>();
    switch(g) {
        case 1:
            if (active_type == SummonType::Active){
                std::println("Shikigami is already fully manifested");
                return;
            }
            c.summon_type = SummonType::Active;
            break;
        case 2:
            if (active_type == SummonType::Support){
                std::println("Shikigami is already partially manifested");
                return;
            }
            c.summon_type = SummonType::Support;
            break;
        case 3:
            if (active_type == SummonType::Shadow){
                std::println("Shikigami is already dismissed");
                return;
            }
            c.summon_type = SummonType::Shadow;
            break;
        default:
            break;
    }
}