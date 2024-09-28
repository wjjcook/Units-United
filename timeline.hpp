#ifndef TIMELINE_H
#define TIMELINE_H

#include <deque>

#include "units/unit.hpp"

class Timeline {
    public:
        Timeline();
        ~Timeline();
        std::deque<Unit*> getUnits();
        Unit* getUnit(int index);
        unsigned int size();

        void append(Unit* unit);
        void updateAliveUnits();

    private:
        std::deque<Unit*> gameUnits;

};

#endif