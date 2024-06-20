#include "unit.hpp"
#include <iostream>
#include <random>

Unit::Unit() : gen(rd()) {
    basicAttack = "Basic Attack";
    
}

Unit::~Unit() {}

void Unit::attack() {
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    std::cout << distr(gen) << std::endl;
}