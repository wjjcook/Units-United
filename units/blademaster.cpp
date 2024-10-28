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
    int firstAttack = distr(gen);
    game.unitAttack(this, victim, firstAttack);

    std::uniform_int_distribution<> multiHits(0, 3);
    
    if (multiHits(gen) == 3) {
        std::vector<int> multiAttacks = {firstAttack};
        std::vector<PassiveEventMessage> events = onAttackPassives(victim);
        game.sendPassiveEvents(events);
        for (PassiveEventMessage event : events) {
            multiAttacks.push_back(event.getValue());
        }
        game.updateUIAfterAttack(this, victim, firstAttack, game.generateCustomAnnouncement(victim, multiAttacks));
    }
}

std::vector<PassiveEventMessage> Blademaster::onAttackPassives(Unit* victim){
    std::vector<PassiveEventMessage> events;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);

    int secondAttack = distr(gen);
    int thirdAttack = distr(gen);

    victim->damageUnit(secondAttack + thirdAttack);

    events.push_back(PassiveEventMessage(victim->getName(), "additionalAttack", secondAttack));
    events.push_back(PassiveEventMessage(victim->getName(), "additionalAttack", thirdAttack));

    return events;
}