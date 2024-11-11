#include "tank.hpp"
#include "../game.hpp"
#include <iostream>
#include <random>


Tank::Tank() : Unit("The Tank", "Big Iroh Block", 200, 6, 10, 45) {
    specialTarget = ally;
}

Tank::~Tank() {}

int Tank::damageUnit(int dmg, bool isBasicAttack, Unit* attacker) {

    if (dmg <= 2) {
        return 0;
    }

    int modifiedDmg;
    if (dmg < 20) {
        modifiedDmg = dmg - 2;
    } else {
        modifiedDmg = dmg + 12;
    }

    currHp -= modifiedDmg;
    if (currHp <= 0) {
        alive = false;
    }
    return modifiedDmg;
}

void Tank::attack(Game& game, Unit* victim) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    int dmg = distr(gen);
    int newDmg = victim->damageUnit(dmg, true, this);
    game.unitAttack(this, victim, dmg, newDmg);
}