#include "../../header/Sorcery/Technique.hpp"

#include <print>
#include <cstdint>
#include <stdexcept>

void Technique::PrintName() const {
    std::println("{}{}{}", identity.color, identity.name, identity.color.empty() ? "" : "\x1b[0m");
}

void Technique::PrintAbilities() const {
    std::uint8_t z = 0;
    for ([[maybe_unused]] const auto& [damage, cost, output, t_identity, at_type] : abilities){
        std::println("{}:[{}{}{}] {:.1f} damage |{:.1f} cursed energy cost |{:.1f} output cost", 
            ++z, t_identity.color, t_identity.name, t_identity.color.empty() ? "" : "\x1b[0m", damage, cost, output);
    }
}
TechAbility Technique::GetAbility(size_t idx){
    if (idx >= abilities.size()){
        throw std::invalid_argument("Invalid Value");
    }
    return abilities[idx];
}
size_t Technique::GetAbility() const{
    return abilities.size();
}