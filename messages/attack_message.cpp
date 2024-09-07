#include "attack_message.hpp"

AttackMessage::AttackMessage(){
    type = MessageType::ATTACK;
}

AttackMessage::AttackMessage(int attackerId, int targetId, int damage) {
    type = MessageType::ATTACK;
    this->attackerId = attackerId;
    this->targetId = targetId;
    this->damage = damage;
}

MessageType AttackMessage::getType() const {
    return type;
}

int AttackMessage::getAttackerId() {
    return attackerId;
}

int AttackMessage::getTargetId() {
    return targetId;
}

int AttackMessage::getDamage() {
    return damage;
}

void AttackMessage::serialize(char* buffer) const {
    size_t offset = 0;
    memcpy(buffer, &type, sizeof(type));
    offset += sizeof(type);

    memcpy(buffer + offset, &attackerId, sizeof(attackerId));
    offset += sizeof(attackerId);

    memcpy(buffer + offset, &targetId, sizeof(targetId));
    offset += sizeof(targetId);

    memcpy(buffer + offset, &damage, sizeof(damage));
}

void AttackMessage::deserialize(const char* buffer) {
    size_t offset = sizeof(MessageType);
    memcpy(&attackerId, buffer + offset, sizeof(attackerId));
    offset += sizeof(attackerId);

    memcpy(&targetId, buffer + offset, sizeof(targetId));
    offset += sizeof(targetId);

    memcpy(&damage, buffer + offset, sizeof(damage));
}