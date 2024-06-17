#include "player.hpp"
#include <iostream>

Player::Player() {
    mana = 0;
    std::cout << "created";
}

Player::~Player() {
    std::cout << "destroyed";
}

