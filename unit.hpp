#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <random>

class Unit {
    public:
        Unit();
        ~Unit();
        virtual void attack();

    protected:
        std::string name;
        static std::string basicAttack;
        std::string specialName;
        int hp;
        int minDmg;
        int maxDmg;
        int speed;

        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen; // seed the generator
};

#endif