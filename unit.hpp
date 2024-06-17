#ifndef UNIT_H
#define UNIT_H

class Unit {
public:
    Unit();
    ~Unit();

protected:
    int hp;
    int minDmg;
    int maxDmg;
    int speed;
};

#endif