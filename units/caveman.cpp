#include "caveman.hpp"
#include <iostream>
#include <random>


Caveman::Caveman() : Unit("The Caveman", "Giga Club", 170, 7, 13, 10) {}

Caveman::~Caveman() {}

int Caveman::attack() const{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    return distr(gen);
} 