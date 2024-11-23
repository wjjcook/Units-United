#ifndef ATTACK_MESSAGE_H
#define ATTACK_MESSAGE_H

#include <vector>

#include "message.hpp"

class AttackMessage : public Message {
    public:
        AttackMessage();
        AttackMessage(int attackerId, int targetId, int damage);

        MessageType getType() const override;
        int getAttackerId();
        int getTargetId();
        int getDamage();
        void serialize(char* buffer) const override;
        void deserialize(const char* buffer) override;

    private:
        int attackerId;
        int targetId;
        int damage;
};

#endif