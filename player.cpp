#include "player.hpp"
#include <iostream>
#include <algorithm>

Player::Player() {
    mana = 0;
}

Player::~Player() {
}

std::vector<Unit*> Player::getUnits() {
    return units;
}

void Player::addUnit(Unit* newUnit, int playerNum) {
    newUnit->setPlayerNum(playerNum);
    units.push_back(newUnit);
    unitNames.insert(newUnit->getName());
}

bool Player::hasUnit(const std::string& unitName) const {
    return unitNames.find(unitName) != unitNames.end();
}

void Player::sortUnitsBySpeed() {
    std::sort(units.begin(), units.end(), [](Unit* lhs, Unit* rhs) { return lhs->getSpeed() > rhs->getSpeed(); });
}