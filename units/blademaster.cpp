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
    int newDmg = victim->damageUnit(firstAttack, true, this);
    game.unitAttack(this, victim, firstAttack, newDmg);

    std::uniform_int_distribution<> multiHits(0, 3);
    
    if (multiHits(gen) == 3) {
        newSecondAttack = 0;
        newThirdAttack = 0;
        std::vector<int> multiAttacks = {newDmg};
        std::vector<PassiveEventMessage> events = onAttackPassives(victim);
        game.sendPassiveEvents(events);
        multiAttacks.push_back(newSecondAttack);
        multiAttacks.push_back(newThirdAttack);
        game.updateUIAfterAttack(this, victim, firstAttack, game.generateCustomAnnouncement(victim, multiAttacks));
    } else {
        game.sendPassiveEvents({PassiveEventMessage(name, "END", 0)});
    }
}

std::vector<PassiveEventMessage> Blademaster::onAttackPassives(Unit* victim){
    std::vector<PassiveEventMessage> events;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minDmg, maxDmg);

    int secondAttack = distr(gen);
    int thirdAttack = distr(gen);

    newSecondAttack = victim->damageUnit(secondAttack, true, this);
    newThirdAttack = victim->damageUnit(thirdAttack, true, this);

    events.push_back(PassiveEventMessage(victim->getName(), "additionalAttack", secondAttack));
    events.push_back(PassiveEventMessage(victim->getName(), "additionalAttack", thirdAttack));
    events.push_back(PassiveEventMessage(name, "END", 0));

    return events;
}