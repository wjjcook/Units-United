#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <set>
#include "unit.hpp"

class Player {
    public:
        Player();
        ~Player();

        std::vector<Unit*> getUnits();
        void addUnit(Unit* newUnit);
        bool hasUnit(const std::string& unitName) const;

    private:
        std::vector<Unit*> units;
        std::set<std::string> unitNames;
        int mana;
        
};

#endif