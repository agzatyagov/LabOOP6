#include "visitor.h"

bool CanKill(NPCType attacker, NPCType defender)
{
    if (attacker == NPCType::SlaveTrader && defender == NPCType::Squirrel) return true;
    if (attacker == NPCType::WanderingKnight && defender == NPCType::SlaveTrader) return true;
    if (attacker == NPCType::Squirrel && defender == NPCType::Squirrel) return true;
    return false;
}

FightVisitor::FightVisitor(NPCType attackerType_, const std::string& attackerName_)
{
    attackerType = attackerType_;
    attackerName = attackerName_;

    defenderType = NPCType::Unknown;
    defenderName = "";

    attackerKills = false;
    defenderKills = false;
}

void FightVisitor::compute()
{
    attackerKills = CanKill(attackerType, defenderType);//тру - атаккер убил дефендера
    defenderKills = CanKill(defenderType, attackerType);//тру - дефендер убил аттакера
}

bool FightVisitor::getAttackerKills() const { return attackerKills; }
bool FightVisitor::getDefenderKills() const { return defenderKills; }

NPCType FightVisitor::getDefenderType() const { return defenderType; }
std::string FightVisitor::getDefenderName() const { return defenderName; }

void FightVisitor::visit(SlaveTrader& npc)
{
    defenderType = NPCType::SlaveTrader;
    defenderName = npc.getName();
    compute();
}

void FightVisitor::visit(Squirrel& npc)
{
    defenderType = NPCType::Squirrel;
    defenderName = npc.getName();
    compute();
}

void FightVisitor::visit(WanderingKnight& npc)
{
    defenderType = NPCType::WanderingKnight;
    defenderName = npc.getName();
    compute();
}
