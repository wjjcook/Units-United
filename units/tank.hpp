#ifndef TANK_H
#define TANK_H

#include "unit.hpp"

class Tank: public Unit {
    public:
        Tank();
        ~Tank() override;
        void attack(Game& game, Unit* victim) override;

    private:
};


#endif