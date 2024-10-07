#ifndef TANK_H
#define TANK_H

#include "unit.hpp"

class Tank: public Unit {
    public:
        Tank();
        ~Tank() override;
        int attack() override;
        int onAttackPassives(int dmg) override;

    private:
};


#endif