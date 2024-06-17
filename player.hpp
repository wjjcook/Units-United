#ifndef PLAYER_H
#define PLAYER_H

#include "unit.hpp"

class Player {
public:
    Player();
    ~Player();

private:
    Unit units[4];
    int mana;
    
};

#endif