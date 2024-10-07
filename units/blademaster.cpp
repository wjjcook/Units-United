#include "blademaster.hpp"
#include <iostream>
#include <random>


Blademaster::Blademaster() : Unit("The Blademaster", "Guillotine", 120, 9, 13, 85) {}

Blademaster::~Blademaster() {}

int Blademaster::attack(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    return distr(gen);
} 

int Blademaster::onAttackPassives(int dmg) {
    return 0;
}