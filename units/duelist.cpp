#include "duelist.hpp"
#include "../game.hpp"
#include <iostream>
#include <random>

Duelist::Duelist() : Unit("The Duelist", "Agni Kai", 110, 6, 10, 60) {}

Duelist::~Duelist() {}

void Duelist::attack(Game& game, Unit* victim){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    game.unitAttack(this, victim, distr(gen));
} 

std::vector<PassiveEventMessage> Duelist::onAttackPassives(){
    std::vector<PassiveEventMessage> events;
    return events;
}