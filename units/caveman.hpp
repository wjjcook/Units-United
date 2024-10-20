#ifndef CAVEMAN_H
#define CAVEMAN_H

#include "unit.hpp"

class Caveman: public Unit {
    public:
        Caveman();
        ~Caveman() override;
        void attack(Game& game, Unit* victim) override;
        std::vector<PassiveEventMessage> onAttackPassives() override;

    private:
};


#endif