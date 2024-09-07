#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <random>

enum Special_Target {
    enemy, ally
};

class Unit {
    public:
        Unit(const std::string name, const std::string specialName, int hp, int minDmg, int maxDmg, int speed);
        virtual ~Unit();

        std::string getName();
        std::string getSpecialName();
        Special_Target getSpecialTarget();
        int getMaxHp();
        int getCurrHp();
        int getSpeed();
        int getId();
        int getPlayerNum();

        void setCurrHp(int hp);
        void setId(int id);
        void setPlayerNum(int playerNum);

        void damageUnit(int dmg);

        virtual int attack() const;

    protected:
        std::string name;
        std::string basicAttack;
        std::string specialName;
        Special_Target specialTarget;
        int maxHp;
        int currHp;
        int minDmg;
        int maxDmg;
        int speed;

        int id;
        int playerNum;
        bool alive;

        
};

#endif