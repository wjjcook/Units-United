#ifndef VILLAGE_IDIOT_H
#define VILLAGE_IDIOT_H

#include "unit.hpp"

class VillageIdiot: public Unit {
    public:
        VillageIdiot();
        ~VillageIdiot() override;
        int attack() override;
        int onAttackPassives(int dmg) override;

    private:
};


#endif