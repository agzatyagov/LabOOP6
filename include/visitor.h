#pragma once
#include <string>
#include "npc.h"

class NPCVisitor
{
public:
    virtual ~NPCVisitor() {}
    virtual void visit(SlaveTrader& npc) = 0;
    virtual void visit(Squirrel& npc) = 0;
    virtual void visit(WanderingKnight& npc) = 0;
};

bool CanKill(NPCType attacker, NPCType defender);

class FightVisitor : public NPCVisitor
{
private:
    NPCType attackerType;
    std::string attackerName;

    NPCType defenderType;
    std::string defenderName;

    bool attackerKills;
    bool defenderKills;

    void compute();

public:
    FightVisitor(NPCType attackerType_, const std::string& attackerName_);

    bool getAttackerKills() const;
    bool getDefenderKills() const;

    NPCType getDefenderType() const;
    std::string getDefenderName() const;

    void visit(SlaveTrader& npc) override;
    void visit(Squirrel& npc) override;
    void visit(WanderingKnight& npc) override;
};
