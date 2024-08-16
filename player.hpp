#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <set>
#include "units/unit.hpp"

class Player {
    public:
        Player();
        ~Player();

        bool isLocalPlayer();
        void setLocalPlayer(bool lp);
        int getMana();
        std::vector<Unit*> getUnits();
        std::set<std::string> getUnitNames();
        void addUnit(Unit* newUnit, int playerNum);
        bool hasUnit(const std::string& unitName) const;
        void sortUnitsBySpeed();

    private:
        bool localPlayer;
        std::vector<Unit*> units;
        std::set<std::string> unitNames;
        int mana;
                
};

#endif