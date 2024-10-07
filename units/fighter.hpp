#ifndef FIGHTER_H
#define FIGHTER_H

#include "unit.hpp"

class Fighter: public Unit {
    public:
        Fighter();
        ~Fighter() override;
        int attack() override;
        int onAttackPassives(int dmg) override;

    private:
};


#endif