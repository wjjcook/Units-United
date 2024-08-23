#ifndef MEDIC_H
#define MEDIC_H

#include "unit.hpp"

class Medic: public Unit {
    public:
        Medic();
        ~Medic() override;
        int attack() const override;

    private:
        int minHeal;
        int maxHeal;
};


#endif