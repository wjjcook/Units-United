#ifndef BLADEMASTER_H
#define BLADEMASTER_H

#include "unit.hpp"

class Blademaster: public Unit {
    public:
        Blademaster();
        ~Blademaster() override;
        int attack() override;
        void onAttackPassives(int dmg) override;

    private:
};


#endif