#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <vector>

#include "orc.hpp"
#include "bear.hpp"
#include "squirrel.hpp"

namespace {
int next_id() {
    static int id = 0;
    return ++id;
}

std::shared_ptr<NPC> make_npc(NpcType type) {
    const std::string base = "npc_" + std::to_string(next_id());
    switch (type) {
    case OrcType:
        return std::make_shared<Orc>(base, 0, 0);
    case SquirrelType:
        return std::make_shared<Squirrel>(base, 0, 0);
    case BearType:
        return std::make_shared<Bear>(base, 0, 0);
    default:
        throw std::invalid_argument("Unknown NPC type");
    }
}
}

TEST(NpcConstruction, ClampsCoordinates) {
    Bear b("clamp_bear", NPC::MAX_COORD + 50, NPC::MIN_COORD - 50);
    EXPECT_EQ(NPC::MAX_COORD, b.x);
    EXPECT_EQ(NPC::MIN_COORD, b.y);
}

TEST(NpcConstruction, UniqueNamesEnforced) {
    Squirrel first("duplicate", 10, 10);
    EXPECT_THROW(Squirrel second("duplicate", 20, 20), std::invalid_argument);
}

TEST(NpcGeometry, DistanceCheckInclusive) {
    auto a = std::make_shared<Orc>("orc_distance", 0, 0);
    auto b = std::make_shared<Bear>("bear_distance", 3, 4);
    EXPECT_TRUE(a->is_close(b, 5));
    EXPECT_TRUE(b->is_close(a, 5));
    EXPECT_FALSE(a->is_close(b, 4));
}

TEST(NpcFight, FightMatrix) {
    struct Expectation {
        NpcType attacker;
        NpcType defender;
        FightResult result;
    };

    const std::vector<Expectation> cases{
        {OrcType, BearType, FightResult::DefenderDead},
        {OrcType, OrcType, FightResult::BothDead},
        {BearType, SquirrelType, FightResult::DefenderDead},
        {BearType, OrcType, FightResult::None},
        {SquirrelType, SquirrelType, FightResult::BothDead},
        {SquirrelType, BearType, FightResult::None},
        {OrcType, SquirrelType, FightResult::None}
    };

    for (const auto &c : cases) {
        auto attacker = make_npc(c.attacker);
        auto defender = make_npc(c.defender);
        EXPECT_EQ(c.result, defender->accept(attacker))
            << "Attacker " << static_cast<int>(c.attacker)
            << " defender " << static_cast<int>(c.defender);
    }
}
