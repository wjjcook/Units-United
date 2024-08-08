#include "character_selection_message.hpp"

CharacterSelectionMessage::CharacterSelectionMessage(){}

CharacterSelectionMessage::CharacterSelectionMessage(std::vector<std::string> selectedUnits) : units(selectedUnits) {}

MessageType CharacterSelectionMessage::getType() const {
    return MessageType::CHARACTER_SELECTION;
}

void CharacterSelectionMessage::serialize(char* buffer) const {
    memcpy(buffer, &units, sizeof(units));
}

void CharacterSelectionMessage::deserialize(const char* buffer) {
    memcpy(&units, buffer, sizeof(units));
}