#include "string_message.hpp"

StringMessage::StringMessage(){
    type = MessageType::STRING;
}

StringMessage::StringMessage(std::string s) : str(s) {
    type = MessageType::STRING;
}

MessageType StringMessage::getType() const {
    return type;
}

std::string StringMessage::getString() {
    return str;
}

void StringMessage::setString(std::string s) {
    str = s;
}

void StringMessage::serialize(char* buffer) const {
    size_t offset = sizeof(size_t);

    memcpy(buffer + offset, &type, sizeof(type));
    offset += sizeof(type);

    size_t stringLength = str.size();
    memcpy(buffer + offset, &stringLength, sizeof(stringLength));
    offset += sizeof(stringLength);

    memcpy(buffer + offset, str.c_str(), stringLength);

    size_t messageSize = offset - sizeof(size_t);
    memcpy(buffer, &messageSize, sizeof(messageSize));
}

void StringMessage::deserialize(const char* buffer) {
    size_t offset = sizeof(MessageType);

    size_t stringLength = str.size();
    memcpy(&stringLength, buffer + offset, sizeof(stringLength));
    offset += sizeof(stringLength);

    str.assign(buffer + offset, stringLength);
}