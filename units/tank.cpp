#include "tank.hpp"
#include <iostream>
#include <random>


Tank::Tank() : Unit("The Tank", "Big Iroh Block", 200, 6, 10, 45) {
    specialTarget = ally;
}

Tank::~Tank() {}

int Tank::attack() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    return distr(gen);
} 

int Tank::onAttackPassives(int dmg) {
    return 0;
}