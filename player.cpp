#include "player.hpp"
#include <iostream>
#include <algorithm>

Player::Player() {
    localPlayer = false;
    mana = 5;
    lost = false;
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

void Player::addUnit(Unit* newUnit, int playerNum) {
    newUnit->setPlayerNum(playerNum);
    units.push_back(newUnit);
}

bool Player::hasUnit(const std::string& unitName) const {
    for (unsigned int i = 0; i < units.size(); i++) {
        if (units[i]->getName() == unitName) {
            return true;
        }
    }
    return false;
}

void Player::sortUnitsBySpeed() {
    std::sort(units.begin(), units.end(), [](Unit* lhs, Unit* rhs) { return lhs->getSpeed() > rhs->getSpeed(); });
}

bool Player::areAllUnitsAlive() {
    for (unsigned int i = 0; i < units.size(); i++) {
        if (units[i]->isAlive()) {
            return true;
        }
    }
    lost = true;
    return false;
}

int Player::aliveUnitCount() {
    int count = 0;
    for (unsigned int i = 0; i < units.size(); i++) {
        if (units[i]->isAlive()) {
            count++;
        }
    }
    return count;
}