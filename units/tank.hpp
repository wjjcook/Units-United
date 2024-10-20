#ifndef TANK_H
#define TANK_H

#include "unit.hpp"

class Tank: public Unit {
    public:
        Tank();
        ~Tank() override;
        void attack(Game& game, Unit* victim) override;
        std::vector<PassiveEventMessage> onAttackPassives() override;

    private:
};


#endif