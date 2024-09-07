#include "unit.hpp"
#include <iostream>
#include <random>
#include <string>

Unit::Unit(std::string name, const std::string specialName, int hp, int minDmg, int maxDmg, int speed) {
    this->name = name;
    this->specialName = specialName;
    maxHp = hp;
    currHp = hp;
    this->minDmg = minDmg;
    this->maxDmg = maxDmg;
    this->speed = speed;
    basicAttack = "Basic Attack";
    specialTarget = enemy;
    alive = true;
}

Unit::~Unit() {}

std::string Unit::getName() {
    return name;
}

std::string Unit::getSpecialName() {
    return specialName;
}

Special_Target Unit::getSpecialTarget() {
    return specialTarget;
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

int Unit::getId(){
    return id;
}

int Unit::getPlayerNum(){
    return playerNum;
}

void Unit::setCurrHp(int hp) {
    currHp = hp;
}

void Unit::setId(int id) {
    this->id = id;
}

void Unit::setPlayerNum(int pNum) {
    playerNum = pNum;
}

void Unit::damageUnit(int dmg) {
    currHp -= dmg;
    if (currHp <= 0) {
        alive = false;
    }
}

int Unit::attack() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    return distr(gen);
}