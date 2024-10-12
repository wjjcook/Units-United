#include "fighter.hpp"
#include "../game.hpp"
#include <iostream>
#include <random>


Fighter::Fighter() : Unit("The Fighter", "Haymaker", 135, 5, 8, 75) {}

Fighter::~Fighter() {}

void Fighter::attack(Game& game, Unit* victim) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    game.unitAttack(this, victim, distr(gen));
} 