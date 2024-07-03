#include "unit.hpp"
#include <iostream>
#include <random>
#include <string>

Unit::Unit(std::string name, const std::string specialName, int hp, int minDmg, int maxDmg, int speed) {
    this->name = name;
    this->specialName = specialName;
    this->maxHp = hp;
    this->currHp = hp;
    this->minDmg = minDmg;
    this->maxDmg = maxDmg;
    this->speed = speed;
    basicAttack = "Basic Attack";
}

Unit::~Unit() {}

std::string Unit::getName() {
    return name;
}

int Unit::getMaxHp() {
    return maxHp;
}

int Unit::getCurrHp() {
    return currHp;
}

int Unit::getSpeed() {
    return speed;
}

int Unit::getPlayerNum(){
    return playerNum;
}

void Unit::setPlayerNum(int pNum) {
    playerNum = pNum;
}

void Unit::attack() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    std::cout << distr(gen) << std::endl;
}