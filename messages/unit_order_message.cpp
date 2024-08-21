#include "unit_order_message.hpp"

UnitOrderMessage::UnitOrderMessage(){
    type = MessageType::UNIT_ORDER;
}

UnitOrderMessage::UnitOrderMessage(std::vector<std::pair<std::string, int>> selectedUnits) : units(selectedUnits) {
    type = MessageType::UNIT_ORDER;
}

MessageType UnitOrderMessage::getType() const {
    return type;
}

std::vector<std::pair<std::string, int>> UnitOrderMessage::getUnits() {
    return units;
}

void UnitOrderMessage::setUnits(std::vector<std::pair<std::string, int>> selectedUnits) {
    units = selectedUnits;
}

void UnitOrderMessage::serialize(char* buffer) const {
    size_t offset = 0;
    memcpy(buffer, &type, sizeof(type));
    offset += sizeof(type);

    // Serialize the size of the vector
    size_t vectorSize = units.size();
    memcpy(buffer + offset, &vectorSize, sizeof(vectorSize));
    offset += sizeof(vectorSize);

    // Serialize each pair in the vector
    for (const auto& pair : units) {
        // Serialize the string (unit name)
        size_t nameLength = pair.first.size();
        memcpy(buffer + offset, &nameLength, sizeof(nameLength));
        offset += sizeof(nameLength);
        memcpy(buffer + offset, pair.first.c_str(), nameLength);
        offset += nameLength;

        // Serialize the int (player number)
        memcpy(buffer + offset, &pair.second, sizeof(pair.second));
        offset += sizeof(pair.second);
    }
}

void UnitOrderMessage::deserialize(const char* buffer) {
    size_t offset = sizeof(MessageType);

    // Deserialize the size of the vector
    size_t vectorSize;
    memcpy(&vectorSize, buffer + offset, sizeof(vectorSize));
    offset += sizeof(vectorSize);

    // Clear the existing vector
    units.clear();

    // Deserialize each pair in the vector
    for (size_t i = 0; i < vectorSize; ++i) {
        // Deserialize the string (unit name)
        size_t nameLength;
        memcpy(&nameLength, buffer + offset, sizeof(nameLength));
        offset += sizeof(nameLength);
        std::string unitName(buffer + offset, nameLength);
        offset += nameLength;

        // Deserialize the int (player number)
        int playerNumber;
        memcpy(&playerNumber, buffer + offset, sizeof(playerNumber));
        offset += sizeof(playerNumber);

        // Insert the pair into the vector
        units.emplace_back(unitName, playerNumber);
    }
}