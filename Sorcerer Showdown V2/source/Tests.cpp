#include "../header/CharacterType/CurseUser.hpp"
#include "../header/Creator.hpp"
#include "../header/Battlefield.hpp"
#include "../header/Systems/System/DomainSystem.hpp"
#include "../header/Systems/BattlefieldSystem.hpp"
#include "../header/Systems/System/CombatSystem.hpp"
#include "../header/Systems/System/TechniqueSystem.hpp"
#include "../header/Sorcery/Technique.hpp"
#include "../header/Sorcery/Domain.hpp"

#include <gtest/gtest.h>
#include <optional>

TEST(DomainSystemTest, AttritionWinCheck){
    std::optional<Domain> d1 = Domain{.damage = 50.0,.is_active = true};
    std::optional<Domain> d2 = Domain{.health = 1.0, .is_active = true};
    const auto winner = DomainSystem::ClashDomains(d1, d2);
    ASSERT_EQ(winner.second, DomainWinCon::Attrition);
}

TEST(DomainSystemTest, RefinementWinCheck){
    std::optional<Domain> d1 = Domain{.is_active = true, .refinement = Refinement::Absolue};
    std::optional<Domain> d2 = Domain{.is_active = true, .refinement = Refinement::Brittle};
    const auto winner = DomainSystem::ClashDomains(d1, d2);
    ASSERT_EQ(winner.first, ClashWinner::First);
}
TEST(DomainSystemTest, RangeWinCheck){
    std::optional<Domain> d1 = Domain{.range = 50, .is_active = true};
    std::optional<Domain> d2 = Domain{.range = 10, .is_active = true};
    const auto winner = DomainSystem::ClashDomains(d1, d2);
    ASSERT_EQ(winner.second, DomainWinCon::Overwhelmed);
}

TEST(BattlefieldSystemTest, SingleDomainSurehitCheck){
    Battlefield bf;
    bf.battlefield.push_back(Create::Gojo());
    bf.battlefield.push_back(Create::Gojo());
    std::optional<Domain> d1 = Domain{.damage = 5000.0, .is_active = true, .damage_type = globalums::DamageType::BypassAll};
    static_cast<CurseUser*>(bf.battlefield[0].get())->Jujutsu().domain = d1;
    BattlefieldSystem::HandleDomainInteraction(bf);
    const auto& t = bf.battlefield[1].get();
    ASSERT_TRUE(t->State().health < 0.0);
}

TEST(TechniqueSystemTest, ProperDamageCheck) {
    std::optional<Technique> t = Technique{};
    t->Abilities().push_back({.id{"Lime Green"}, .damage = 50.0, .cost = 0.0, .output = 0.0});
    std::unique_ptr<CurseUser> c1 = std::make_unique<CurseUser>();
    std::unique_ptr<Character> c2 = std::make_unique<Character>();
    c2->State().max_health = 50.0;
    c2->State().health = c2->State().max_health;
    c2->State().durability = 0.0;
    CombatSystem::ResolveTechnique(*c1, t->Abilities()[0], *c2);
    EXPECT_DOUBLE_EQ(c2->State().health, 0.0);
}
TEST(TechniqueSystemTest, DamageNegationCheck){
    std::optional<Technique> t = Technique{};
    t->Abilities().push_back({.id{"Lime Green"}, .damage = 50.0, .cost = 0.0, .output = 0.0});
    std::unique_ptr<CurseUser> c1 = std::make_unique<CurseUser>();
    std::unique_ptr<Character> c2 = std::make_unique<Character>();
    c2->State().max_health = 50.0;
    c2->State().health = c2->State().max_health;
    c2->State().durability = 100.0;
    CombatSystem::ResolveTechnique(*c1, t->Abilities()[0], *c2);
    EXPECT_NE(c2->State().health, 0.0);
}
TEST(TechniqueSystemTest, BarrierCheck){
    std::unique_ptr<Character> c1 = std::make_unique<Character>();
    std::unique_ptr<CurseUser> c2 = std::make_unique<CurseUser>();
    c1->State().strength = 100000.0;
    c2->Jujutsu().technique = Create::Limitless();
    c2->State().durability = 0.0;
    CombatSystem::ResolveAttacking(*c1, *c2);
    ASSERT_DOUBLE_EQ(c2->State().health, c2->State().max_health);
}

TEST(CombatSystemTest, BlackFlashTest){
    std::unique_ptr<CurseUser> c1 = std::make_unique<CurseUser>();
    std::unique_ptr<Character> c2 = std::make_unique<Character>();
    c1->CursedEnergy().bf_chance = 100;
    const auto r = CombatSystem::ResolveAttacking(*c1, *c2);
    ASSERT_TRUE(r.is_blackflash);

}
TEST(CombatSystemTest, BlackFlashDamageTest){
    std::unique_ptr<CurseUser> c1 = std::make_unique<CurseUser>();
    std::unique_ptr<Character> c2 = std::make_unique<Character>();
    c1->CursedEnergy().bf_chance = 100; // 2.5 multiplier with blackflash
    c1->State().strength = 50.0; // should be 125.0 damage
    c2->State().max_health = 250.0;
    c2->State().health = c2->State().max_health;
    c2->State().durability = 0.0;
    CombatSystem::ResolveAttacking(*c1, *c2);
    ASSERT_DOUBLE_EQ(c2->State().health, 125.0);
}
TEST(CombatSystemTest, BasicDamageTest){
    std::unique_ptr<Character> c1 = std::make_unique<Character>();// should be both auto initialized with all doubles set to 1.0
    std::unique_ptr<Character> c2 = std::make_unique<Character>(); 
    c2->State().durability = 0.0;
    CombatSystem::ResolveAttacking(*c1, *c2);
    ASSERT_DOUBLE_EQ(c2->State().health, 0.0);
}