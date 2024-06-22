#include "tank.hpp"
#include <iostream>
#include <random>


Tank::Tank() : Unit("The Tank", "Big Iroh Block", 200, 6, 10, 45) {}

Tank::~Tank() {}

void Tank::attack() const{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    std::cout << distr(gen) << std::endl;
} 