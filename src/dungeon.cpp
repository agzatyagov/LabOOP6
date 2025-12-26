#include "dungeon.h"
#include "visitor.h"
#include <iostream>
#include <fstream>

static std::string MakeKillMessage(NPCType killerType,const std::string& killerName,NPCType victimType,//вывод имени и типа убийца и убитого
    const std::string& victimName)
{
    std::string msg;
    msg = TypeToString(killerType) + " " + killerName + " killed " +
        TypeToString(victimType) + " " + victimName;
    return msg;
}

static double sqr(double v) { 
    return v * v; 
}

bool Dungeon::nameExists(const std::string& name) const
{
    size_t i = 0;
    while (i < npcs.size())
    {
        if (npcs[i]->getName() == name) return true;
        i = i + 1;
    }
    return false;
}

bool Dungeon::coordsOk(int x, int y) const
{
    if (x < 0 || x > 500) return false;
    if (y < 0 || y > 500) return false;
    return true;
}

const std::vector<std::shared_ptr<NPC>>& Dungeon::getAll() const
{
    return npcs;
}

bool Dungeon::addNPC(NPCType type, const std::string& name, int x, int y, NPCFactory& factory)
{
    if (type == NPCType::Unknown) return false;
    if (!coordsOk(x, y)) return false;
    if (name.size() == 0) return false;
    if (nameExists(name)) return false;

    std::shared_ptr<NPC> npc = factory.create(type, name, x, y);
    if (npc.get() == 0) return false;

    npcs.push_back(npc);
    return true;
}

bool Dungeon::saveToFile(const std::string& fileName) const
{
    std::ofstream out(fileName.c_str());
    if (!out.is_open()) return false;

    size_t i = 0;
    while (i < npcs.size())
    {
        out << TypeToString(npcs[i]->getType()) << " "<< npcs[i]->getName() << " "<< npcs[i]->getX() << " "
            << npcs[i]->getY() << "\n";
        i = i + 1;
    }
    return true;
}

bool Dungeon::loadFromFile(const std::string& fileName, NPCFactory& factory)
{
    std::ifstream in(fileName.c_str());
    if (!in.is_open()) return false;

    std::vector<std::shared_ptr<NPC>> loaded;

    while (true)
    {
        std::string typeStr;
        std::string name;
        int x = 0, y = 0;

        in >> typeStr;
        if (!in.good()) break;

        in >> name >> x >> y;

        NPCType t = StringToType(typeStr);
        if (t == NPCType::Unknown) continue;
        if (x < 0 || x > 500 || y < 0 || y > 500) continue;

        bool dup = false;// проверка на уникальность имени при загрузке
        size_t i = 0;
        while (i < loaded.size())
        {
            if (loaded[i]->getName() == name) { 
                dup = true; 
                break;
            }
            i = i + 1;
        }
        if (dup) continue;

        std::shared_ptr<NPC> npc = factory.create(t, name, x, y);
        if (npc.get() == 0) continue;

        loaded.push_back(npc);
    }

    npcs = loaded;
    return true;
}

void Dungeon::print() const
{
    if (npcs.size() == 0)
    {
        std::cout << "No NPC\n";
        return;
    }

    size_t i = 0;
    while (i < npcs.size())
    {
        std::cout << TypeToString(npcs[i]->getType()) << " "<< npcs[i]->getName() << " "<< "x=" << npcs[i]->getX() << " "
            << "y=" << npcs[i]->getY() << "\n";
        i = i + 1;
    }
}

void Dungeon::battle(double range, BattleSubject& subject)
{
    if (range < 0.0) return;

    double r2 = range * range;//квадрат введённого рэнджа

    std::vector<int> dead;//массив мертвых нпс
    dead.resize(npcs.size());
    size_t i = 0;
    while (i < dead.size()) { 
        dead[i] = 0; 
        i = i + 1; 
    }

    size_t a = 0;
    while (a < npcs.size())//перебор аттакеров
    {
        if (dead[a] != 0) { 
            a = a + 1; 
            continue; 
        }

        size_t b = a + 1;//перебор защищающихся=>перебор возможных пар боев
        while (b < npcs.size())
        {
            if (dead[b] != 0) { 
                b = b + 1; 
                continue; 
            }

            double dx = (double)npcs[a]->getX() - (double)npcs[b]->getX();//вычисление квадрата расстояния между нпс
            double dy = (double)npcs[a]->getY() - (double)npcs[b]->getY();
            double d2 = sqr(dx) + sqr(dy);

            if (d2 > r2) { 
                b = b + 1; 
                continue; 
            }

            NPCType attackerType = npcs[a]->getType();
            std::string attackerName = npcs[a]->getName();

            FightVisitor v(attackerType, attackerName);
            npcs[b]->accept(v);

            if (v.getAttackerKills())//если убил аттакер
            {
                subject.notifyKill(MakeKillMessage(attackerType, attackerName,v.getDefenderType(), v.getDefenderName()));
                dead[b] = 1;
            }

            if (v.getDefenderKills())//если убил защищающийся
            {
                subject.notifyKill(MakeKillMessage(v.getDefenderType(), v.getDefenderName(),attackerType, attackerName));
                dead[a] = 1;
                break;
            }

            b = b + 1;
        }

        a = a + 1;
    }

    std::vector<std::shared_ptr<NPC>> alive;
    size_t k = 0;
    while (k < npcs.size())
    {
        if (dead[k] == 0) alive.push_back(npcs[k]);
        k = k + 1;
    }
    npcs = alive;
}
