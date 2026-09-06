#include "../../header/Editors/TechniqueEditor.hpp"
#include "../../header/Sorcery/Technique.hpp"

void TechniqueEditor::SetName(Technique& t, std::string_view n){
    t.identity.name = n;
}
void TechniqueEditor::SetColor(Technique& t, std::string_view c){
    t.identity.color = c;
}
void TechniqueEditor::SetDescription(Technique& t, std::string_view d){ // unused for now / debatable inclusion
    t.identity.description = d;
}
void TechniqueEditor::AddAbility(Technique& t, const TechAbility& tb){
    t.abilities.push_back(tb);
}