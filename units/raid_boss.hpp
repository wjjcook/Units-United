#ifndef RAID_BOSS_H
#define RAID_BOSS_H

#include "unit.hpp"

class RaidBoss: public Unit {
    public:
        RaidBoss();
        ~RaidBoss() override;
        void attack(Game& game, Unit* victim) override;
        std::vector<PassiveEventMessage> onAttackPassives() override;

    private:
};


#endif