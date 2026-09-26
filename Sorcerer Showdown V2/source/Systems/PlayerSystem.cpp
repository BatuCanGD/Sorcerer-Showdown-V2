#include "../../header/Systems/PlayerSystem.hpp"
#include "../../header/Utilities/Input.hpp"
#include "../../header/Battlefield.hpp"
#include "../../header/Systems/System/CombatSystem.hpp"
#include "../../header/Systems/System/TechniqueSystem.hpp"
#include "../../header/Logger.hpp"
#include "../../header/CharacterType/Character.hpp"
#include "../../header/CharacterType/CurseUser.hpp"

#include <string_view>
#include <optional>
#include <print>


std::vector<Action> UserControl::GetChoices(const Character& c) {
    std::vector<Action> ac;
    int t{0};
    auto add_ch([&](std::string_view sv, Action a){
        std::println("{}:{}", ++t, sv);
        ac.push_back(a);
    });

    add_ch("Attack", Action::Attack);

    if (c.Equipment().current_tool || c.Equipment().stored_tool || (c.Equipment().has_access_to_inventory && !c.Equipment().inventory.empty()))
        add_ch("Inventory", Action::Inventory);

    if (const auto& cr = c.CanUseSorcery()){
        if (cr->Jujutsu().technique)
            add_ch("Technique", Action::Technique);
        if (cr->Jujutsu().domain)
            add_ch("Domain", Action::Domain);
        if (!cr->Jujutsu().shikigami.empty())
            add_ch("Shikigami", Action::Shikigami);
        add_ch("Sorcery", Action::Sorcery);
    }
    return ac;
}
void UserControl::GetPlayerTurn(Character& c, Battlefield& bf) {
    const auto choices = UserControl::GetChoices(c);
    auto* crs = c.CanUseSorcery();
    bool looping = true;
    do{
        size_t idx = get_input<size_t>() - 1;
        while (idx >= choices.size()) {
            std::println("Invalid Input");
            idx = get_input<size_t>() - 1;
        }
        if (UserControl::GetConfirmation()){
            switch(choices[idx]){
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
        std::println("{}:{} | HP: {}", ++z, c->Name(), c->Health());
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
        std::println("{}:{}{}{}", ++ww, w.identity.color, w.identity.name,
                      w.identity.color.empty() ? "" : "\x1b[0m");
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
    std::println("Chosen Tool: [{}{}{}]", chosen.tool->identity.color, chosen.tool->identity.name, chosen.tool->identity.color.empty() ? "" : "\x1b[0m");
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
    if (c.RCTSystem().can_use_rct){
        add_p("RCT", SorceryType::RCT);
    }
    add_p("Reinforcement", SorceryType::Reinforcement);
    add_p("Binding Vows", SorceryType::BindingVows);
    return sp;
}


void UserControl::ForRCT(CurseUser& c){
    std::println("total usage | cost"); // placeholders

    std::println("set | do nothing");

    std::println("new cost, are you sure?");
}
void UserControl::ForReinforcement(CurseUser& c){
    std::println("total usage | cost"); // placeholders

    std::println("set | do nothing");

    std::println("new cost, are you sure?");
}
void UserControl::ForBindingVows(CurseUser& c){
    std::println("currently used vows | description"); // placeholders

    std::println("add | remove | do nothing");

    std::println("choose | return");
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
    const auto& domain = c->Jujutsu().domain;
    const auto& neutralizer = c->Jujutsu().domain_neutralizer;

    if (!(domain && neutralizer)) {
        std::println("You do not have a domain or anything that can neutralize it");
        return true;
    }
    if (domain){
        std::println("[{}{}{}] - {}", domain->identity.color, domain->identity.name, domain->identity.color.empty() ? "" : "\x1b[0m", domain->is_active ? "Active" : "Inactive");
    }
    if (neutralizer) {
        std::println("[{}{}{}] - {}", neutralizer->identity.color, neutralizer->identity.name, neutralizer->identity.color.empty() ? "" : "\x1b[0m", neutralizer->is_active ? "Active" : "Inactive");
    }

    return false;
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
        std::println("{}:{}{}{}", ++idx, x.id.color, x.id.name, x.id.color.empty() ? "" : "\x1b[0m");
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
    std::println("Chosen: {}{}{} -  {}\n1 - Manifest\n2 - Partial Manifestation\n3 - Dismiss", 
        active,
        c.id.color, 
        c.id.name, 
        c.id.color.empty() ? "" : "\x1b[0m"
    );
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