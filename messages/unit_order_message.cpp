#include "unit_order_message.hpp"

UnitOrderMessage::UnitOrderMessage(){
    type = MessageType::UNIT_ORDER;
}

UnitOrderMessage::UnitOrderMessage(std::map<std::string, int> selectedUnits) : units(selectedUnits) {
    type = MessageType::UNIT_ORDER;
}

MessageType UnitOrderMessage::getType() const {
    return type;
}

std::map<std::string, int> UnitOrderMessage::getUnits() {
    return units;
}

void UnitOrderMessage::setUnits(std::map<std::string, int> selectedUnits) {
    units = selectedUnits;
}

void UnitOrderMessage::serialize(char* buffer) const {
    size_t offset = 0;
    memcpy(buffer, &type, sizeof(type));
    offset += sizeof(type);

    // Serialize the size of the map
    size_t mapSize = units.size();
    memcpy(buffer + offset, &mapSize, sizeof(mapSize));
    offset += sizeof(mapSize);

    // Serialize each key-value pair
    for (const auto& pair : units) {
        // Serialize the key (string)
        size_t keyLength = pair.first.size();
        memcpy(buffer + offset, &keyLength, sizeof(keyLength));
        offset += sizeof(keyLength);
        memcpy(buffer + offset, pair.first.c_str(), keyLength);
        offset += keyLength;

        // Serialize the value (int)
        memcpy(buffer + offset, &pair.second, sizeof(pair.second));
        offset += sizeof(pair.second);
    }
}

void UnitOrderMessage::deserialize(const char* buffer) {
    size_t offset = sizeof(MessageType);
    size_t mapSize;
    memcpy(&mapSize, buffer + offset, sizeof(mapSize));
    offset += sizeof(mapSize);

    // Clear the existing map
    units.clear();

    // Deserialize each key-value pair
    for (size_t i = 0; i < mapSize; ++i) {
        // Deserialize the key (string)
        size_t keyLength;
        memcpy(&keyLength, buffer + offset, sizeof(keyLength));
        offset += sizeof(keyLength);
        std::string key(buffer + offset, keyLength);
        offset += keyLength;

        // Deserialize the value (int)
        int value;
        memcpy(&value, buffer + offset, sizeof(value));
        offset += sizeof(value);

        // Insert the key-value pair into the map
        units[key] = value;
    }
}