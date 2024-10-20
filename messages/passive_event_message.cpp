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

std::string PassiveEventMessage::getUnitName() {
    return unitName;
}

std::string PassiveEventMessage::getPassiveType() {
    return passiveType;
}

int PassiveEventMessage::getValue() {
    return value;
}

void PassiveEventMessage::serialize(char* buffer) const {
    size_t offset = 0;
    memcpy(buffer, &type, sizeof(type));
    offset += sizeof(type);

    size_t stringLength = unitName.size();
    memcpy(buffer + offset, &stringLength, sizeof(stringLength));
    offset += sizeof(stringLength);
    memcpy(buffer + offset, unitName.c_str(), stringLength);
    offset += stringLength;

    stringLength = passiveType.size();
    memcpy(buffer + offset, &stringLength, sizeof(stringLength));
    offset += sizeof(stringLength);
    memcpy(buffer + offset, passiveType.c_str(), stringLength);
    offset += stringLength;

    memcpy(buffer + offset, &value , sizeof(value));
}

void PassiveEventMessage::deserialize(const char* buffer) {
    size_t offset = sizeof(MessageType);

    size_t stringLength = unitName.size();
    memcpy(&stringLength, buffer + offset, sizeof(stringLength));
    offset += sizeof(stringLength);
    unitName.assign(buffer + offset, stringLength);
    offset += stringLength;

    stringLength = passiveType.size();
    memcpy(&stringLength, buffer + offset, sizeof(stringLength));
    offset += sizeof(stringLength);
    passiveType.assign(buffer + offset, stringLength);
    offset += stringLength;

    memcpy(&value, buffer + offset, sizeof(value));
}