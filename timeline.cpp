#include "timeline.hpp"

Timeline::Timeline(){}

std::deque<Unit*> Timeline::getUnits() {
    return gameUnits;
}

void Timeline::append(Unit* unit) {
    gameUnits.push_back(unit);
}