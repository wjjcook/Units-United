#ifndef DUELIST_H
#define DUELIST_H

#include "unit.hpp"

class Duelist: public Unit {
    public:
        Duelist();
        ~Duelist() override;
        void attack() const override;

    private:
};


#endif