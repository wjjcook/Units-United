#ifndef FIGHTER_H
#define FIGHTER_H

#include "unit.hpp"

class Fighter: public Unit {
    public:
        Fighter();
        ~Fighter();
        void attack() const override;

    private:
};


#endif