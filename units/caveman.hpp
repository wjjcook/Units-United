#ifndef CAVEMAN_H
#define CAVEMAN_H

#include "unit.hpp"

class Caveman: public Unit {
    public:
        Caveman();
        ~Caveman() override;
        int attack() override;
        void onAttackPassives(int dmg) override;

    private:
};


#endif