#include "player.hpp"
#include <iostream>

Player::Player() {
    mana = 0;
}

Player::~Player() {
}

std::vector<Unit> Player::getUnits() {
    return units;
}
