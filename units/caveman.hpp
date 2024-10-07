#ifndef CAVEMAN_H
#define CAVEMAN_H

#include "unit.hpp"

class Caveman: public Unit {
    public:
        Caveman();
        ~Caveman() override;
        int attack() override;
        int onAttackPassives(int dmg) override;

    private:
};


#endif