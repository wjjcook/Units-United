#include "village_idiot.hpp"
#include "../game.hpp"
#include <iostream>
#include <random>


VillageIdiot::VillageIdiot() : Unit("The Village Idiot", "Simply Fall Over", 120, 5, 9, 30) {}

VillageIdiot::~VillageIdiot() {}

void VillageIdiot::attack(Game& game, Unit* victim) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    game.unitAttack(this, victim, distr(gen));
} 

std::vector<PassiveEventMessage> VillageIdiot::onAttackPassives(Unit* victim){
    std::vector<PassiveEventMessage> events;
    return events;
}