#include "village_idiot.hpp"
#include <iostream>
#include <random>


VillageIdiot::VillageIdiot() : Unit("The Village Idiot", "Simply Fall Over", 120, 5, 9, 30) {}

VillageIdiot::~VillageIdiot() {}

int VillageIdiot::attack() const{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    return distr(gen);
} 