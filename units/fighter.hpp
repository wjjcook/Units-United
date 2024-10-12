#ifndef FIGHTER_H
#define FIGHTER_H

#include "unit.hpp"

class Fighter: public Unit {
    public:
        Fighter();
        ~Fighter() override;
        void attack(Game& game, Unit* victim) override;

    private:
};


#endif