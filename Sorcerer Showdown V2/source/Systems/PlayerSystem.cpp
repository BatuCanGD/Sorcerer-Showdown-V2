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
    bool looping = true;
    do{
        for (const auto& str : choices.second){
            std::println("{}", str);
        }
        size_t idx = get_input<size_t>() - 1;
        while (idx >= choices.first.size()) {
            std::println("Invalid Input");
            idx = get_input<size_t>() - 1;
        }
        if (UserControl::GetConfirmation()){
            switch(choices.first[idx]){
                case Action::Attack:
                    looping = UserControl::DoAttack(c, *UserControl::GetUserTarget(bf));
                    break;
                case Action::Inventory:
                    looping = UserControl::DoInventoryManagement(c);
                    break;
                case Action::Technique:
                    looping = UserControl::DoTechnique(crs, *UserControl::GetUserTarget(bf));
                    break;
                case Action::Domain:
                    looping = UserControl::DoDomain(crs);
                    break;
                case Action::Shikigami:
                    looping = UserControl::DoShikigami(crs);
                    break;
                case Action::Sorcery:
                    looping = UserControl::DoSorcery(crs);
                    break;
                default: 
                    break;
            }
        }

    } while (looping);
}

bool UserControl::GetConfirmation() {
    std::println("Are you sure?\n 1 - Yes | 2 - No");
    return get_input<int>() == 1;
}

Character* UserControl::GetUserTarget(Battlefield& bf) {
    size_t z{0};
    for (const auto& c : bf.battlefield){
        std::println("{0}:{1} | HP: {2:.1f}", ++z, GetInfo::Name(c->Identity()), c->State().health);
    }
    std::println("Pick a target");
    size_t x = get_input<size_t>() - 1;
    while (x >= bf.battlefield.size()){
        std::println("Try again");
        x = get_input<size_t>() - 1;
    }
    return bf.battlefield[x].get();
}

bool UserControl::DoAttack(Character& c, Character& cd) {
    const auto result = c.Attack(cd);
    Log::Attack(result, c, cd);
    return false;
}

bool UserControl::DoInventoryManagement(Character& c) {
    auto& equip = c.Equipment();
    const bool has_inv = equip.has_access_to_inventory;

    enum class Origin { 
        MainHand,
        OffHand,
        Inventory
    };
    struct Candidate { 
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
        return true;
    }

    size_t input = get_input<size_t>() - 1;
    if (input >= wp.size()) {
        std::println("Invalid Input"); 
        return true;
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
                return true;
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
                return true;
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
                return true;
            }
            equip.inventory.push_back(extract_chosen());
            break;
        }
        default: return true;
    }
    return true;
}

bool UserControl::DoSorcery(CurseUser* c) {
    if (c == nullptr){
        std::println("You are not a curse user!");
        return true;
    }
    const auto& sp = UserControl::GetSorceryChoices(*c);
    size_t ch = get_input<size_t>() - 1;

    if (ch >= sp.size()) {
        return true;
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
    return true;
}

const std::vector<UserControl::SorceryType> UserControl::GetSorceryChoices(const CurseUser& c) {
    std::vector<SorceryType> sp;
    int k{0};
    auto add_p([&](std::string_view sv, SorceryType s){
        std::println("{} - {} |", ++k, sv);
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
    std::println("Total RCT output: {0:.1f} | Cost: {1:.1f} CE per turn\n1 - Set Output | 2 - Return", rct, SorcerySystem::ApplyRCTCost(rct));
    if (get_input<int>() != 1) return;
    std::print("Enter Output Amount: ");
    const double prv = rct;
    rct = std::max(get_input<double>(), 0.0);

    std::println("New cost: {:.1f}", SorcerySystem::ApplyRCTCost(rct));
    if (!UserControl::GetConfirmation()){
        rct = prv;
    }
}
void UserControl::ForReinforcement(CurseUser& c){
    double& rf = c.CursedEnergy().reinforcement_amount;
    std::println("Total Usage: {0:.1f} | Cost: {0:.1f}", rf, SorcerySystem::ApplyReinforcementCost(rf));
    std::println("Output: {0:.1f}/{1:.1f}", c.Output().current_output, c.Output().max_output_potential);

    std::println("1 - Set | 2 - Return");
    if (get_input<int>() != 1){
        return;
    }
    std::print("Enter Reinforcement Amount: ");
    const double prv = rf;
    rf = std::max(get_input<double>(), 0.0);

    std::println("New cost: {:.1f}", SorcerySystem::ApplyReinforcementCost(rf));
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
    std::println("How much will you sacrifice?\n 100-1/0.01-0.99\n=> ");
    double dp = get_input<double>();
    if (dp < 0.01) {
        std::println("You must sacrifice at least 1%");
        return;
    } else if (dp > 99.9 || (dp > 0.99 && dp < 1.0)){
        std::println("You cannot just sacrifice 100% or more");
        return;
    }
    const double sdp = (dp >= 1.0 && dp <= 99.9) ? dp * 0.01 : dp;

    bv.gain = gain;
    bv.loss = sack;
    bv.percentage = sdp;
    bv.identity = VowSystem::GenerateVowId(bv);
    c.Jujutsu().binding_vows.push_back(bv);
}


bool UserControl::DoTechnique(CurseUser* c, Character& cd) {
    if (c == nullptr){
        std::println("You are not a curse user!");
        return true;
    }
    if (!c->Jujutsu().technique.has_value()) {
        std::println("You do not have a technique");
        return true;
    }
    const auto& ability = TechniqueSystem::ChooseAbility(*c->Jujutsu().technique);
    const auto result = CombatSystem::ResolveTechnique(*c, ability, cd);
    Log::TechniqueAttack(result);
    return false;
}
bool UserControl::DoDomain(CurseUser* c) {
    if (c == nullptr){
        std::println("You are not a curse user!");
        return true;
    } 
    auto& domain = c->Jujutsu().domain;
    auto& neutralizer = c->Jujutsu().neutralizer;

    if (!domain && !neutralizer) {
        std::println("You do not have a domain or anything that can neutralize it");
        return true;
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
        std::println("[{}] - {}", GetInfo::Name(domain->identity), domain->is_active ? "Active" : "Inactive");
    }
    if (neutralizer) {
        add_ch(dmn::Neutralizer);
        std::println("[{}] - {}", GetInfo::Name(neutralizer->identity), neutralizer->is_active ? "Active" : "Inactive"); 
    }

    size_t pl = get_input<size_t>() - 1;
    if (pl >= dm.size()){
        return true;
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
            }else {
                return true;
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
            }else {
                return true;
            }
            break;
        }
    }

    return true;
}

//
// Shikigami player Functions
//

bool UserControl::DoShikigami(CurseUser* c) {
    if (c == nullptr){
        std::println("You are not a curse user!");
        return true;
    }
    if (c->Jujutsu().shikigami.empty()) {
        std::println("You do not have any shikigami");
        return true;
    }
    UserControl::UseShikigami(*UserControl::ChooseShikigami(c->Jujutsu().shikigami));
    return true;
}

Shikigami* UserControl::ChooseShikigami(std::vector<Shikigami>& sh) {
    size_t idx{0};
    for (const auto& x : sh){
        std::println("{}:{}", ++idx, GetInfo::Name(x.id));
    }

    idx = get_input<size_t>() - 1;

    while (idx >= sh.size()) {
        std::println("Invalid Input");
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