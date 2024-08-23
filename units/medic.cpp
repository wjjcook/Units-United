#include "medic.hpp"
#include <iostream>
#include <random>


Medic::Medic() : Unit("The Medic", "Health Pack", 110, 1, 4, 15) {
    minHeal = 6;
    maxHeal = 8;
    specialTarget = ally;
}

Medic::~Medic() {}

int Medic::attack() const{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    return distr(gen);
} 