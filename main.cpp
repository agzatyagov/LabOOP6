#include <iostream>
#include <string>
#include "dungeon.h"
#include "npc_factory.h"
#include "observer.h"

static void PrintMenu()
{
    std::cout << "\n";
    std::cout << "1 - Add NPC\n";
    std::cout << "2 - Save to file\n";
    std::cout << "3 - Load from file\n";
    std::cout << "4 - Print NPC list\n";
    std::cout << "5 - Battle mode\n";
    std::cout << "0 - Exit\n";
}

int main()
{
    Dungeon dungeon;
    NPCFactory factory;

    BattleSubject subject;
    ConsoleObserver consoleObs;
    FileObserver fileObs("log.txt");
    subject.attach(&consoleObs);
    subject.attach(&fileObs);

    while (true)
    {
        PrintMenu();

        int cmd = -1;
        std::cin >> cmd;

        if (cmd == 0) break;

        if (cmd == 1)
        {
            int typeInt = 0;
            std::string name;
            int x = 0, y = 0;

            std::cout << "Choose type:\n";
            std::cout << "1 - SlaveTrader\n";
            std::cout << "2 - Squirrel\n";
            std::cout << "3 - WanderingKnight\n";
            std::cin >> typeInt;

            std::cout << "Enter unique name (no spaces):\n";
            std::cin >> name;

            std::cout << "Enter x (0..500):\n";
            std::cin >> x;

            std::cout << "Enter y (0..500):\n";
            std::cin >> y;

            NPCType type = (NPCType)typeInt;
            bool ok = dungeon.addNPC(type, name, x, y, factory);
            std::cout << (ok ? "Added\n" : "Add error\n");
            continue;
        }

        if (cmd == 2)
        {
            std::string fileName;
            std::cout << "File name:\n";
            std::cin >> fileName;
            std::cout << (dungeon.saveToFile(fileName) ? "Saved\n" : "Save error\n");
            continue;
        }

        if (cmd == 3)
        {
            std::string fileName;
            std::cout << "File name:\n";
            std::cin >> fileName;
            std::cout << (dungeon.loadFromFile(fileName, factory) ? "Loaded\n" : "Load error\n");
            continue;
        }

        if (cmd == 4)
        {
            dungeon.print();
            continue;
        }

        if (cmd == 5)
        {
            double range = 0.0;
            std::cout << "Enter battle range:\n";
            std::cin >> range;

            dungeon.battle(range, subject);

            std::cout << "After battle:\n";
            dungeon.print();
            continue;
        }

        std::cout << "Unknown command\n";
    }

    return 0;
}
