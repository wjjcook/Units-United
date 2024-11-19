#include "fighter.hpp"
#include "../game.hpp"
#include <iostream>
#include <random>


Fighter::Fighter() : Unit("The Fighter", "Haymaker", 135, 5, 8, 75) {
    standingGround = false;
}

Fighter::~Fighter() {}

void Fighter::setStandingGround(bool sg) {
    standingGround = sg;
}

int Fighter::damageUnit(int dmg, bool isBasicAttack, Unit* attacker) {
    int totalDmg = dmg;
    if (standingGround) {
        totalDmg -= 3;
        if (totalDmg < 0) {
            totalDmg = 0;
        }
    }
    
    currHp -= totalDmg;
    if (currHp <= 0) {
        alive = false;
    }

    return totalDmg;
}

void Fighter::attack(Game& game, Unit* victim) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    int dmg = distr(gen);
    int newDmg = victim->damageUnit(dmg, true, this);
    game.unitAttack(this, victim, dmg, newDmg);
    game.sendPassiveEvents({PassiveEventMessage(name, "END", 0)});
} 

std::vector<PassiveEventMessage> Fighter::onAttackPassives(Unit* victim){
    std::vector<PassiveEventMessage> events;
    events.push_back(PassiveEventMessage(name, "END", 0));
    return events;
}