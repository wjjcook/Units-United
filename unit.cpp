#include "unit.hpp"
#include <iostream>
#include <random>

Unit::Unit() {
    basicAttack = "Basic Attack";
    std::random_device rd;
    std::mt19937 gen(rd());
}

Unit::~Unit() {}

void Unit::attack() {

}