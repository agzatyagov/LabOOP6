#pragma once
#include <memory>
#include <string>
#include <vector>
#include "npc.h"
#include "npc_factory.h"
#include "observer.h"

class Dungeon
{
private:
    std::vector<std::shared_ptr<NPC>> npcs;//массив живых нпс

    bool nameExists(const std::string& name) const;//уникальность имени
    bool coordsOk(int x, int y) const;//коорды в пределах 500x500

public:
    const std::vector<std::shared_ptr<NPC>>& getAll() const;

    bool addNPC(NPCType type, const std::string& name, int x, int y, NPCFactory& factory);//добавить нпс 1

    bool saveToFile(const std::string& fileName) const;//сохранить в файл 2
    bool loadFromFile(const std::string& fileName, NPCFactory& factory);//загрузить из файла  3

    void print() const;//вывод живых нпс 4

    void battle(double range, BattleSubject& subject);//режим драки 5
};
