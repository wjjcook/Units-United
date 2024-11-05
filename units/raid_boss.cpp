#include "raid_boss.hpp"
#include "../game.hpp"
#include <iostream>
#include <random>


RaidBoss::RaidBoss() : Unit("The Raid Boss", "Earthshatter", 200, 15, 20, 1) {
    overwhelmed = 0;
}

RaidBoss::~RaidBoss() {}

int RaidBoss::damageUnit(int dmg) {
    int totalDmg = dmg + overwhelmed;
    currHp -= totalDmg;
    if (currHp <= 0) {
        alive = false;
    }
    overwhelmed += 1;
    return totalDmg;
}

void RaidBoss::attack(Game& game, Unit* victim) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> hits(0, 1);

    if (hits(gen) == 0) {
        game.unitAttack(this, victim, 0, 0);
        return;
    }

    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    int dmg = distr(gen);
    int newDmg = victim->damageUnit(dmg);
    game.unitAttack(this, victim, dmg, newDmg);
}

std::vector<PassiveEventMessage> RaidBoss::onAttackPassives(Unit* victim){
    std::vector<PassiveEventMessage> events;
    return events;
}