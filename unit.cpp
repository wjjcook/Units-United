#include "unit.hpp"
#include <iostream>
#include <random>
#include <string>

Unit::Unit(const std::string name, const std::string specialName, int hp, int minDmg, int maxDmg, int speed) {
    this->name = name;
    this->specialName = specialName;
    this->hp = hp;
    this->minDmg = minDmg;
    this->maxDmg = maxDmg;
    this->speed = speed;
    basicAttack = "Basic Attack";
}

Unit::~Unit() {}

void Unit::attack() const{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    std::cout << distr(gen) << std::endl;
}