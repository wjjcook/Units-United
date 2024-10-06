#include "duelist.hpp"
#include <iostream>
#include <random>

Duelist::Duelist() : Unit("The Duelist", "Agni Kai", 110, 6, 10, 60) {}

Duelist::~Duelist() {}

int Duelist::attack(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    return distr(gen);
} 

void Duelist::onAttackPassives(int dmg) {
    return;
}