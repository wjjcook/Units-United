#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "unit.hpp"

class Player {
    public:
        Player();
        ~Player();

    private:
        std::vector<Unit> units;
        int mana;
        
};

#endif