#include "medic.hpp"
#include "../game.hpp"
#include <iostream>
#include <random>


Medic::Medic() : Unit("The Medic", "Health Pack", 110, 1, 4, 15) {
    minHeal = 6;
    maxHeal = 8;
    specialTarget = ally;
}

Medic::~Medic() {}

void Medic::attack(Game& game, Unit* victim) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    game.unitAttack(this, victim, distr(gen));
} 

std::vector<PassiveEventMessage> Medic::onAttackPassives(Unit* victim){
    std::vector<PassiveEventMessage> events;
    return events;
}