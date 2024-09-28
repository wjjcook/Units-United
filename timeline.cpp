#include "timeline.hpp"
#include <iostream>
#include <typeinfo>
Timeline::Timeline(){}

Timeline::~Timeline(){
    for (unsigned int i = 0; i < gameUnits.size(); i++) {
        delete gameUnits[i];
    }
}

std::deque<Unit*> Timeline::getUnits() {
    return gameUnits;
}

Unit* Timeline::getUnit(int index) {
    return gameUnits[index];
}

unsigned int Timeline::size() {
    return gameUnits.size();
}

void Timeline::append(Unit* unit) {
    gameUnits.push_back(unit);
}

void Timeline::updateAliveUnits() {
    for (auto it = gameUnits.begin(); it != gameUnits.end(); ) {
        if (!(*it)->isAlive()) {
            it = gameUnits.erase(it);
        } else {
            ++it;
        }
    }
}