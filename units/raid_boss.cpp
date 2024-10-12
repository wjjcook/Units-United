#include "raid_boss.hpp"
#include "../game.hpp"
#include <iostream>
#include <random>


RaidBoss::RaidBoss() : Unit("The Raid Boss", "Earthshatter", 200, 15, 20, 1) {}

RaidBoss::~RaidBoss() {}

void RaidBoss::attack(Game& game, Unit* victim) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    game.unitAttack(this, victim, distr(gen));
}