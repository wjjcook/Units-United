#ifndef DUELIST_H
#define DUELIST_H

#include "unit.hpp"

class Duelist: public Unit {
    public:
        Duelist();
        ~Duelist() override;
        void attack(Game& game, Unit* victim) override;
        std::vector<PassiveEventMessage> onAttackPassives(Unit* victim) override;

    private:
};


#endif