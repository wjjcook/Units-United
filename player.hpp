#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "units/unit.hpp"

class Player {
    public:
        Player();
        ~Player();

        bool isLocalPlayer();
        void setLocalPlayer(bool lp);
        int getMana();
        std::vector<Unit*> getUnits();
        void addUnit(Unit* newUnit, int playerNum);
        bool hasUnit(const std::string& unitName) const;
        void sortUnitsBySpeed();

    private:
        bool localPlayer;
        std::vector<Unit*> units;
        int mana;
                
};

#endif