#include "../../header/SystemType/Technique.hpp"

#include <print>
#include <stdexcept>

void Technique::PrintAbilities() const {
    uint8_t z = 0;
    for ([[maybe_unused]] auto [damage, output, name, clr, at_type] : abilities){
        std::println("{}:[{}{}{}] {:.1f} damage | {:.1f} output cost", 
            ++z, clr, name, clr.empty() ? "" : "\x1b[0m", damage, output);
    }
}
TechAbility Technique::GetAbility(size_t idx){
    if (idx >= abilities.size()){
        throw std::invalid_argument("Invalid Value");
    }
    return abilities[idx];
}