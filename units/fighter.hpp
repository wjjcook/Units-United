#ifndef FIGHTER_H
#define FIGHTER_H

#include "unit.hpp"

class Fighter: public Unit {
    public:
        Fighter();
        ~Fighter() override;

        void setStandingGround(bool sg);

        int damageUnit(int dmg, bool isBasicAttack, Unit* attacker) override;
        void attack(Game& game, Unit* victim) override;
        std::vector<PassiveEventMessage> onAttackPassives(Unit* victim) override;

    private:
        bool standingGround;
};


#endif