#ifndef DUELIST_H
#define DUELIST_H

#include "unit.hpp"

class Duelist: public Unit {
    public:
        Duelist();
        ~Duelist() override;
        int attack() override;
        void onAttackPassives(int dmg) override;

    private:
};


#endif