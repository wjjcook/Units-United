#ifndef RAID_BOSS_H
#define RAID_BOSS_H

#include "unit.hpp"

class RaidBoss: public Unit {
    public:
        RaidBoss();
        ~RaidBoss() override;
        int attack() override;
        void onAttackPassives(int dmg) override;

    private:
};


#endif