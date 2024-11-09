#include "fighter.hpp"
#include "../game.hpp"
#include <iostream>
#include <random>


Fighter::Fighter() : Unit("The Fighter", "Haymaker", 135, 5, 8, 75) {}

Fighter::~Fighter() {}

void Fighter::attack(Game& game, Unit* victim) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    int dmg = distr(gen);
    int newDmg = victim->damageUnit(dmg, true, this);
    game.unitAttack(this, victim, dmg, newDmg);
} 

std::vector<PassiveEventMessage> Fighter::onAttackPassives(Unit* victim){
    std::vector<PassiveEventMessage> events;
    return events;
}