#ifndef RAID_BOSS_H
#define RAID_BOSS_H

#include "unit.hpp"

class RaidBoss: public Unit {
    public:
        RaidBoss();
        ~RaidBoss();
        void attack() const override;

    private:
};


#endif