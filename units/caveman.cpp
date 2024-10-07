#include "caveman.hpp"
#include <iostream>
#include <random>


Caveman::Caveman() : Unit("The Caveman", "Giga Club", 170, 9, 13, 10) {}

Caveman::~Caveman() {}

int Caveman::attack(){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> hits(0, 1);

    if (hits(gen) == 0) {
        return 0;
    }
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    return distr(gen);
} 

int Caveman::onAttackPassives(int dmg) {
    if (dmg > 0) {
        minDmg += 3;
        maxDmg += 3;
        currHp += 4;
        if (currHp > maxHp) {
            currHp = maxHp;
        }
    }
    return 0;
}