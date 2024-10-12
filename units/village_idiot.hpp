#ifndef VILLAGE_IDIOT_H
#define VILLAGE_IDIOT_H

#include "unit.hpp"

class VillageIdiot: public Unit {
    public:
        VillageIdiot();
        ~VillageIdiot() override;
        void attack(Game& game, Unit* victim) override;

    private:
};


#endif