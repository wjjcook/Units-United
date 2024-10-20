#include "blademaster.hpp"
#include "../game.hpp"
#include <iostream>
#include <random>


Blademaster::Blademaster() : Unit("The Blademaster", "Guillotine", 120, 9, 13, 85) {}

Blademaster::~Blademaster() {}

void Blademaster::attack(Game& game, Unit* victim){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    game.unitAttack(this, victim, distr(gen));
}

std::vector<PassiveEventMessage> Blademaster::onAttackPassives(){
    std::vector<PassiveEventMessage> events;
    return events;
}