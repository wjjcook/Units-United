#include "character_selection_message.hpp"

CharacterSelectionMessage::CharacterSelectionMessage(){
    type = MessageType::CHARACTER_SELECTION;
}

CharacterSelectionMessage::CharacterSelectionMessage(std::vector<std::string> selectedUnits) : units(selectedUnits) {
    type = MessageType::CHARACTER_SELECTION;
}

MessageType CharacterSelectionMessage::getType() const {
    return type;
}

std::vector<std::string> CharacterSelectionMessage::getUnits() {
    return units;
}

void CharacterSelectionMessage::setUnits(std::vector<std::string> selectedUnits) {
    units = selectedUnits;
}

void CharacterSelectionMessage::serialize(char* buffer) const {
    size_t offset = 0;
    memcpy(buffer, &type, sizeof(type));
    offset += sizeof(type);

    // Serialize the number of units
    size_t numUnits = units.size();
    memcpy(buffer + offset, &numUnits, sizeof(numUnits));
    offset += sizeof(numUnits);

    // Serialize each string in the vector
    for (const std::string& unit : units) {
        size_t length = unit.size();
        memcpy(buffer + offset, &length, sizeof(length));
        offset += sizeof(length);
        memcpy(buffer + offset, unit.c_str(), length);
        offset += length;
    }
}

void CharacterSelectionMessage::deserialize(const char* buffer) {
    size_t offset = sizeof(MessageType);
    size_t numUnits;
    memcpy(&numUnits, buffer + offset, sizeof(numUnits));
    offset += sizeof(numUnits);

    // Clear the existing vector
    units.clear();

    // Deserialize each string in the vector
    for (size_t i = 0; i < numUnits; ++i) {
        size_t length;
        memcpy(&length, buffer + offset, sizeof(length));
        offset += sizeof(length);

        std::string unit(buffer + offset, length);
        units.push_back(unit);
        offset += length;
    }
}