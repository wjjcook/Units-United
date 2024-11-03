#include "tank.hpp"
#include "../game.hpp"
#include <iostream>
#include <random>


Tank::Tank() : Unit("The Tank", "Big Iroh Block", 200, 6, 10, 45) {
    specialTarget = ally;
}

Tank::~Tank() {}

void Tank::attack(Game& game, Unit* victim) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    int dmg = distr(gen);
    victim->damageUnit(dmg);
    game.unitAttack(this, victim, dmg);
} 

std::vector<PassiveEventMessage> Tank::onAttackPassives(Unit* victim){
    std::vector<PassiveEventMessage> events;
    return events;
}