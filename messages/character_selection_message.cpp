#include "character_selection_message.hpp"

CharacterSelectionMessage::CharacterSelectionMessage(){
    type = MessageType::CHARACTER_SELECTION;
}

CharacterSelectionMessage::CharacterSelectionMessage(std::set<std::string> selectedUnits) : units(selectedUnits) {
    type = MessageType::CHARACTER_SELECTION;
}

MessageType CharacterSelectionMessage::getType() const {
    return type;
}

void CharacterSelectionMessage::setUnits(std::set<std::string> selectedUnits) {
    units = selectedUnits;
}

void CharacterSelectionMessage::serialize(char* buffer) const {
    size_t offset = 0;
    memcpy(buffer, &type, sizeof(type));
    offset += sizeof(type);
    memcpy(buffer + offset, &units, sizeof(units));
}

void CharacterSelectionMessage::deserialize(const char* buffer) {
    size_t offset = sizeof(MessageType);
    memcpy(&units, buffer + offset, sizeof(units));
}