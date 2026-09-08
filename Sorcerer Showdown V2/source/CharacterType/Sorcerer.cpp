#include "../../header/CharacterType/Sorcerer.hpp"

void Sorcerer::ReverseCursedTechnique(double amount){
    rct_system.rct_output = amount;
}
void Sorcerer::ReverseCursedTechnique(SorcererSystem::RCTLevel type){
    rct_level = type;
}