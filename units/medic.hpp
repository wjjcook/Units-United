#ifndef MEDIC_H
#define MEDIC_H

#include "unit.hpp"

class Medic: public Unit {
    public:
        Medic();
        ~Medic() override;
        void attack(Game& game, Unit* victim) override;
        std::vector<PassiveEventMessage> onAttackPassives() override;
    private:
        int minHeal;
        int maxHeal;
};


#endif