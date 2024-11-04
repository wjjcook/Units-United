#include "caveman.hpp"
#include "../game.hpp"
#include <iostream>
#include <random>


Caveman::Caveman() : Unit("The Caveman", "Giga Club", 170, 9, 13, 10) {}

Caveman::~Caveman() {}

void Caveman::attack(Game& game, Unit* victim){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> hits(0, 1);

    if (hits(gen) == 0) {
        game.unitAttack(this, victim, 0, 0);
        return;
    }

    std::uniform_int_distribution<> distr(minDmg, maxDmg);
    int dmg = distr(gen);

    minDmg += 3;
    maxDmg += 3;
    currHp += 4;
    if (currHp > maxHp) {
        currHp = maxHp;
    }

    int newDmg = victim->damageUnit(dmg);
    std::vector<PassiveEventMessage> events = victim->onDamagePassives(this, dmg);
    game.unitAttack(this, victim, dmg, newDmg);

    std::vector<PassiveEventMessage> onAttackEvents = onAttackPassives(victim);

    events.insert(events.end(), onAttackEvents.begin(), onAttackEvents.end());
    game.sendPassiveEvents(events);

} 

std::vector<PassiveEventMessage> Caveman::onAttackPassives(Unit* victim){
    std::vector<PassiveEventMessage> events;

    events.push_back(PassiveEventMessage(name, "dmgIncrease", 3));
    events.push_back(PassiveEventMessage(name, "heal", 4));

    return events;
}