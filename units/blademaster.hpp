#ifndef BLADEMASTER_H
#define BLADEMASTER_H

#include "unit.hpp"

class Blademaster: public Unit {
    public:
        Blademaster();
        ~Blademaster() override;
        void attack() const override;

    private:
};


#endif