#include "../../header/Systems/PlayerSystem.hpp"
#include "../../header/Utilities/Input.hpp"
#include "../../header/CharacterType/Character.hpp"
#include "../../header/CharacterType/CurseUser.hpp"

#include <string_view>
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
        if (idx >= choices.size()) {
            std::println("Invalid Input");
            idx = get_input<size_t>();
        }
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
    } while (looping);
}

Character* UserControl::GetUserTarget(Battlefield& bf) {
    
}

bool UserControl::DoAttack(Character& c, Character& cd) {

}
bool UserControl::DoInventoryManagement(Character& c) {

}

bool UserControl::DoSorcery(CurseUser* c) {

}
bool UserControl::DoTechnique(CurseUser* c, Character& cd) {

}
bool UserControl::DoDomain(CurseUser* c) {

}
bool UserControl::DoShikigami(CurseUser* c) {

}