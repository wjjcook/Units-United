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
    int dmg = distr(gen);
    int newDmg = victim->damageUnit(dmg);
    game.unitAttack(this, victim, dmg, newDmg);
} 

std::vector<PassiveEventMessage> Duelist::onAttackPassives(Unit* victim){
    std::vector<PassiveEventMessage> events;
    return events;
}