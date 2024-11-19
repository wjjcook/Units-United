#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <random>

#include "../messages/passive_event_message.hpp" 

enum Special_Target {
    enemy, ally
};

class Game;

class Unit {
    public:
        Unit(const std::string name, const std::string specialName, int hp, int minDmg, int maxDmg, int speed);
        virtual ~Unit();

        std::string getName();
        std::string getSpecialName();
        Special_Target getSpecialTarget();
        int getMaxHp();
        int getCurrHp();
        int getMinDmg();
        int getMaxDmg();
        int getSpeed();
        int getId();
        int getPlayerNum();
        bool isAlive();

        void setCurrHp(int hp);
        void increaseDmg(int dmg);
        void setId(int id);
        void setPlayerNum(int playerNum);

        virtual int damageUnit(int dmg, bool isBasicAttack, Unit* attacker);
        virtual void attack(Game& game, Unit* victim);
        virtual bool attackHits();
        virtual std::vector<PassiveEventMessage> onAttackPassives(Unit* victim);
        virtual std::vector<PassiveEventMessage> onDamagePassives(Unit* attacker, int dmg);
        virtual std::vector<PassiveEventMessage> beforeDamagePassives(Game& game, Unit* attacker);
        virtual void onTurnPassives();

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