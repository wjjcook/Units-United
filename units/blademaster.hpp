#ifndef BLADEMASTER_H
#define BLADEMASTER_H

#include "unit.hpp"

class Blademaster: public Unit {
    public:
        Blademaster();
        ~Blademaster() override;
        void attack(Game& game, Unit* victim) override;
        std::vector<PassiveEventMessage> onAttackPassives(Unit* victim) override;

    private:
};


#endif