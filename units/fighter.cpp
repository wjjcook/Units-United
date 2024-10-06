#include "fighter.hpp"
#include <iostream>
#include <random>


Fighter::Fighter() : Unit("The Fighter", "Haymaker", 135, 5, 8, 75) {}

Fighter::~Fighter() {}

int Fighter::attack() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    return distr(gen);
} 

void Fighter::onAttackPassives(int dmg) {
    return;
}