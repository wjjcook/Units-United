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
    int newDmg = victim->damageUnit(dmg, true, this);
    game.unitAttack(this, victim, dmg, newDmg);
    game.sendPassiveEvents({PassiveEventMessage(name, "END", 0)});
}

std::vector<PassiveEventMessage> Duelist::beforeDamagePassives(Game& game, Unit* attacker) {
    std::vector<PassiveEventMessage> events;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> counter(0, 2);
    if (counter(gen) == 2) {
        std::uniform_int_distribution<> distr(minDmg, maxDmg);
        int dmg = distr(gen);
        int newDmg = attacker->damageUnit(dmg, true, this);
        game.unitAttack(this, attacker, dmg, newDmg);
        std::string customAnnouncement = "The Duelist countered " + attacker->getName() + " for " + std::to_string(newDmg) + " damage!";
        game.updateUIAfterAttack(this, attacker, dmg, customAnnouncement);
        events.push_back(PassiveEventMessage(name, "counterAttack", dmg));
        events.push_back(PassiveEventMessage(name, "END", 0));
    }

    return events;
}