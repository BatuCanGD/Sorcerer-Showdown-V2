#include "../../header/Editors/TechniqueEditor.hpp"
#include "../../header/Sorcery/Technique.hpp"

void TechniqueEditor::SetIdentity(Technique& t, TechIdentity id){
    t.identity = id;
}
void TechniqueEditor::AddAbility(Technique& t, TechAbility& tb){
    t.abilities.push_back(std::move(tb));
}