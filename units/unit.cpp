#include "unit.hpp"
#include "../game.hpp"
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

int Unit::getMinDmg() {
    return minDmg;
}

int Unit::getMaxDmg() {
    return maxDmg;
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

bool Unit::isAlive(){
    if (currHp <= 0) {
        alive = false;
    }
    return alive;
}

void Unit::setCurrHp(int hp) {
    currHp = hp;
    if (currHp > maxHp) {
        currHp = maxHp;
    }
}

void Unit::increaseDmg(int dmg) {
    minDmg += dmg;
    maxDmg += dmg;
}

void Unit::setId(int id) {
    this->id = id;
}

void Unit::setPlayerNum(int pNum) {
    playerNum = pNum;
}

int Unit::damageUnit(int dmg, bool isBasicAttack, Unit* attacker) {
    currHp -= dmg;
    if (currHp <= 0) {
        alive = false;
    }
    return dmg;
}

void Unit::attack(Game& game, Unit* victim) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    int dmg = distr(gen);
    int newDmg = victim->damageUnit(dmg, true, this);
    game.unitAttack(this, victim, dmg, newDmg);
    game.sendPassiveEvents({PassiveEventMessage(name, "END", 0)});
}

bool Unit::attackHits() {
    return true;
}

std::vector<PassiveEventMessage> Unit::onAttackPassives(Unit* victim){
    std::vector<PassiveEventMessage> events;
    return events;
}

std::vector<PassiveEventMessage> Unit::onDamagePassives(Unit* attacker, int dmg) {
    std::vector<PassiveEventMessage> events;
    return events;
}

std::vector<PassiveEventMessage> Unit::beforeDamagePassives(Game& game, Unit* attacker) {
    std::vector<PassiveEventMessage> events;
    events.push_back(PassiveEventMessage(name, "END", 0));
    return events;
}

void Unit::onTurnPassives() {}