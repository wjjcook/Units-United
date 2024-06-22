#include "medic.hpp"
#include <iostream>
#include <random>


Medic::Medic() : Unit("The Medic", "Health Pack", 110, 1, 4, 15) {
    minHeal = 6;
    maxHeal = 8;
}

Medic::~Medic() {}

void Medic::attack() const{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    std::cout << distr(gen) << std::endl;
} 