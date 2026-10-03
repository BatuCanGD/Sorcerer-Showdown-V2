#include "../header/Systems/System/DomainSystem.hpp"
#include "../header/Sorcery/Domain.hpp"

#include <gtest/gtest.h>
#include <optional>


TEST(DomainSystemTest, RefinementWinCheck){
    std::optional<Domain> d1 = Domain{.is_active = true, .refinement = Refinement::Absolue};
    std::optional<Domain> d2 = Domain{.is_active = true, .refinement = Refinement::Brittle};
    const auto winner = DomainSystem::ClashDomains(d1, d2);
    ASSERT_EQ(winner.first, ClashWinner::First);
}