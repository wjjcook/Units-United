#ifndef RAID_BOSS_H
#define RAID_BOSS_H

#include "unit.hpp"

class RaidBoss: public Unit {
    public:
        RaidBoss();
        ~RaidBoss() override;
        int damageUnit(int dmg, bool isBasicAttack, Unit* attacker) override;
        void attack(Game& game, Unit* victim) override;

    private:
        int overwhelmed;
};


#endif