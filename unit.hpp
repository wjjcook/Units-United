#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <random>

class Unit {
    public:
        Unit(const std::string name, const std::string specialName, int hp, int minDmg, int maxDmg, int speed);
        ~Unit();

        std::string getName();
        int getSpeed();
        int getPlayerNum();
        void setPlayerNum(int playerNum);

        virtual void attack() const;

    protected:
        std::string name;
        std::string basicAttack;
        std::string specialName;
        int hp;
        int minDmg;
        int maxDmg;
        int speed;

        int playerNum;

        
};

#endif