#ifndef MEDIC_H
#define MEDIC_H

#include "unit.hpp"

class Medic: public Unit {
    public:
        Medic();
        ~Medic() override;
        int attack() override;
        int onAttackPassives(int dmg) override;

    private:
        int minHeal;
        int maxHeal;
};


#endif