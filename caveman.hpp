#ifndef CAVEMAN_H
#define CAVEMAN_H

#include "unit.hpp"

class Caveman: public Unit {
    public:
        Caveman();
        ~Caveman();
        void attack() const override;

    private:
};


#endif