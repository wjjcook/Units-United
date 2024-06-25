#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <random>

class Unit {
    public:
        Unit(const std::string name, const std::string specialName, int hp, int minDmg, int maxDmg, int speed);
        ~Unit();
        virtual void attack() const;

        std::string getName();

    protected:
        std::string name;
        std::string basicAttack;
        std::string specialName;
        int hp;
        int minDmg;
        int maxDmg;
        int speed;

        
};

#endif