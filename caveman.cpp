#include "caveman.hpp"
#include <iostream>
#include <random>

Caveman::Caveman() : Unit("The Caveman", "Giga Club", 170, 7, 13, 10) {}

void Caveman::attack() const {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    std::cout << distr(gen) << std::endl;
}