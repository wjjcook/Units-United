#include "fighter.hpp"
#include <iostream>
#include <random>


Fighter::Fighter() : Unit("The Fighter", "Haymaker", 135, 5, 8, 75) {}

Fighter::~Fighter() {}

void Fighter::attack() const{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    std::cout << distr(gen) << std::endl;
} 