#ifndef TANK_H
#define TANK_H

#include "unit.hpp"

class Tank: public Unit {
    public:
        Tank();
        ~Tank() override;
        int damageUnit(int dmg, bool isBasicAttack, Unit* attacker) override;
        void attack(Game& game, Unit* victim) override;
        std::vector<PassiveEventMessage> onAttackPassives(Unit* victim) override;

    private:
};


#endif