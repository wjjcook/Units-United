#include "passive_event_message.hpp"

PassiveEventMessage::PassiveEventMessage(){
    type = MessageType::PASSIVE_EVENT;
}

PassiveEventMessage::PassiveEventMessage(std::string unitName, std::string passiveType, int value) {
    type = MessageType::PASSIVE_EVENT;
    this->unitName = unitName;
    this->passiveType = passiveType;
    this->value = value;
}

MessageType PassiveEventMessage::getType() const {
    return type;
}

void PassiveEventMessage::serialize(char* buffer) const {
    size_t offset = 0;
    memcpy(buffer, &type, sizeof(type));
    offset += sizeof(type);

    memcpy(buffer + offset, &unitName, sizeof(unitName));
    offset += sizeof(unitName);

    memcpy(buffer + offset, &passiveType, sizeof(passiveType));
    offset += sizeof(passiveType);

    memcpy(buffer + offset, &value , sizeof(value));
}

void PassiveEventMessage::deserialize(const char* buffer) {
    size_t offset = sizeof(MessageType);
    memcpy(&unitName, buffer + offset, sizeof(unitName));
    offset += sizeof(unitName);

    memcpy(&passiveType, buffer + offset, sizeof(passiveType));
    offset += sizeof(passiveType);

    memcpy(&value, buffer + offset, sizeof(value));
}