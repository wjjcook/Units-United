#ifndef TANK_H
#define TANK_H

#include "unit.hpp"

class Tank: public Unit {
    public:
        Tank();
        ~Tank();
        void attack() const override;

    private:
};


#endif