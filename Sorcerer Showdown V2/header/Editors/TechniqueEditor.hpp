#pragma once

#include <string_view>

class Technique;
class TechAbility;

struct TechniqueEditor final {
    static void SetName(Technique& t, std::string_view n);
    static void SetColor(Technique& t, std::string_view c);
    static void SetDescription(Technique& t, std::string_view d);
    static void AddAbility(Technique& t, const TechAbility& tb);
};