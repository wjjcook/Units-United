#include "duelist.hpp"
#include <iostream>
#include <random>

Duelist::Duelist() : Unit("The Duelist", "Agni Kai", 110, 6, 10, 60) {}

Duelist::~Duelist() {}

void Duelist::attack() const{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    std::cout << distr(gen) << std::endl;
} 