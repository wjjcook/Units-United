#include "player.hpp"
#include <iostream>
#include <algorithm>

Player::Player() {
    localPlayer = false;
    mana = 5;
}

Player::~Player() {
    for (unsigned int i = 0; i < units.size(); i++) {
        delete units[i];
    }
}

bool Player::isLocalPlayer() {
    return localPlayer;
}

void Player::setLocalPlayer(bool lp) {
    localPlayer = lp;
}

int Player::getMana() {
    return mana;
}

std::vector<Unit*> Player::getUnits() {
    return units;
}

std::set<std::string> Player::getUnitNames() {
    return unitNames;
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