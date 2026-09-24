#include "../../header/Systems/PlayerSystem.hpp"
#include "../../header/Utilities/Input.hpp"
#include "../../header/Battlefield.hpp"
#include "../../header/Systems/System/CombatSystem.hpp"
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
    const bool& has_inv = c.Equipment().has_access_to_inventory;
    std::vector<CursedTool*> wp;
    size_t ww{0};

    auto add_wp([&](std::optional<CursedTool> w){
        std::println("{}:{}{}{}", ++ww, w->identity.color, w->identity.name, w->identity.color.empty() ? "" : "\x1b[0m");
        wp.push_back(&*w);
    });

    if (const auto& current = c.Equipment().current_tool){
        add_wp(current);
    }
    if (const auto& stored = c.Equipment().stored_tool){
        add_wp(stored);
    }
    if (has_inv){
        for (const auto& tools : c.Equipment().inventory) {
            add_wp(tools);
        }
    }

    size_t input = get_input<size_t>() - 1;

    if (input >= wp.size()){
        std::println("Invalid Input");
        return true;
    }

    auto& chosen_tool = wp[input];

    std::println("Chosen Tool: [{}]", chosen_tool->identity.color, chosen_tool->identity.name, chosen_tool->identity.color.empty() ? "" : "\x1b[0m");
    std::println("1 - Move to main hand\n2 - Move to Offhand\n{}", has_inv ? "3 - Move to Inventory" : "");

    switch(get_input<int>()){
        case 1:
            if (chosen_tool == &*c.Equipment().current_tool) {
                std::println("You cannot move an item in your hand");
                return true;
            }
            if (auto& w = c.Equipment().current_tool){

            }
        case 2:
            if (chosen_tool == &*c.Equipment().stored_tool){
                std::println("You cannot move an item in your hand");
                return true;
            }
            if (auto& w = c.Equipment().stored_tool){
                
            }
        case 3:
            if (!has_inv){
                return true;
            }
        default:
            return true;
    }



    return true;
}

bool UserControl::DoSorcery(CurseUser* c) {
    if (c == nullptr){
        std::println("You are not a curse user!");
        return true;
    }
    return false;
}
bool UserControl::DoTechnique(CurseUser* c, Character& cd) {
    if (c == nullptr){
        std::println("You are not a curse user!");
        return true;
    }
    return false;
}
bool UserControl::DoDomain(CurseUser* c) {
    if (c == nullptr){
        std::println("You are not a curse user!");
        return true;
    }
    return false;
}
bool UserControl::DoShikigami(CurseUser* c) {
    if (c == nullptr){
        std::println("You are not a curse user!");
        return true;
    }
    return false;
}