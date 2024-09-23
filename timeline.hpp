#ifndef TIMELINE_H
#define TIMELINE_H

#include <deque>

#include "units/unit.hpp"

class Timeline {
    public:
        Timeline();
        std::deque<Unit*> getUnits();

        void append(Unit* unit);

    private:
        std::deque<Unit*> gameUnits;

};

#endif