#include <gtest/gtest.h>
#include <vector>
#include <memory>
#include <string>
#include "dungeon.h"
#include "npc_factory.h"
#include "observer.h"

class TestObserver : public IObserver
{
public:
    std::vector<std::string> messages;
    void onKill(const std::string& message) override { messages.push_back(message); }
};

TEST(FactoryTest, CreateTypes)
{
    NPCFactory factory;

    auto a = factory.create(NPCType::SlaveTrader, "t1", 10, 10);
    auto b = factory.create(NPCType::Squirrel, "s1", 20, 20);
    auto c = factory.create(NPCType::WanderingKnight, "k1", 30, 30);

    ASSERT_TRUE(a.get() != nullptr);
    ASSERT_TRUE(b.get() != nullptr);
    ASSERT_TRUE(c.get() != nullptr);

    EXPECT_EQ(a->getType(), NPCType::SlaveTrader);
    EXPECT_EQ(b->getType(), NPCType::Squirrel);
    EXPECT_EQ(c->getType(), NPCType::WanderingKnight);
}

TEST(DungeonTest, AddUniqueAndCoords)
{
    Dungeon d;
    NPCFactory f;

    EXPECT_TRUE(d.addNPC(NPCType::Squirrel, "a", 0, 0, f));
    EXPECT_FALSE(d.addNPC(NPCType::Squirrel, "a", 1, 1, f));
    EXPECT_FALSE(d.addNPC(NPCType::Squirrel, "b", -1, 1, f));
    EXPECT_FALSE(d.addNPC(NPCType::Unknown, "c", 1, 1, f));
}

TEST(DungeonTest, SaveLoad)
{
    Dungeon d1;
    NPCFactory f;

    ASSERT_TRUE(d1.addNPC(NPCType::SlaveTrader, "t1", 1, 2, f));
    ASSERT_TRUE(d1.addNPC(NPCType::Squirrel, "s1", 3, 4, f));

    ASSERT_TRUE(d1.saveToFile("test_npcs.txt"));

    Dungeon d2;
    ASSERT_TRUE(d2.loadFromFile("test_npcs.txt", f));

    const auto& v = d2.getAll();
    ASSERT_EQ(v.size(), 2u);

    EXPECT_EQ(v[0]->getType(), NPCType::SlaveTrader);
    EXPECT_EQ(v[0]->getName(), "t1");
    EXPECT_EQ(v[0]->getX(), 1);
    EXPECT_EQ(v[0]->getY(), 2);

    EXPECT_EQ(v[1]->getType(), NPCType::Squirrel);
    EXPECT_EQ(v[1]->getName(), "s1");
    EXPECT_EQ(v[1]->getX(), 3);
    EXPECT_EQ(v[1]->getY(), 4);
}

TEST(BattleTest, SlaveTraderKillsSquirrel)
{
    Dungeon d;
    NPCFactory f;

    ASSERT_TRUE(d.addNPC(NPCType::SlaveTrader, "t1", 0, 0, f));
    ASSERT_TRUE(d.addNPC(NPCType::Squirrel, "s1", 0, 1, f));

    BattleSubject subject;
    TestObserver obs;
    subject.attach(&obs);

    d.battle(10.0, subject);

    ASSERT_EQ(d.getAll().size(), 1u);
    EXPECT_EQ(d.getAll()[0]->getType(), NPCType::SlaveTrader);
    ASSERT_EQ(obs.messages.size(), 1u);
}

TEST(BattleTest, KnightKillsSlaveTrader)
{
    Dungeon d;
    NPCFactory f;

    ASSERT_TRUE(d.addNPC(NPCType::WanderingKnight, "k1", 0, 0, f));
    ASSERT_TRUE(d.addNPC(NPCType::SlaveTrader, "t1", 0, 1, f));

    BattleSubject subject;
    TestObserver obs;
    subject.attach(&obs);

    d.battle(10.0, subject);

    ASSERT_EQ(d.getAll().size(), 1u);
    EXPECT_EQ(d.getAll()[0]->getType(), NPCType::WanderingKnight);
    ASSERT_EQ(obs.messages.size(), 1u);
}

TEST(BattleTest, SquirrelsKillEachOther)
{
    Dungeon d;
    NPCFactory f;

    ASSERT_TRUE(d.addNPC(NPCType::Squirrel, "s1", 0, 0, f));
    ASSERT_TRUE(d.addNPC(NPCType::Squirrel, "s2", 0, 1, f));

    BattleSubject subject;
    TestObserver obs;
    subject.attach(&obs);

    d.battle(10.0, subject);

    ASSERT_EQ(d.getAll().size(), 0u);
    ASSERT_EQ(obs.messages.size(), 2u);
}
