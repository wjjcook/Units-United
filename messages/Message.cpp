#include "message.hpp"
#include "character_selection_message.hpp"

Message* Message::createMessage(MessageType type) {
    switch (type) {
        case MessageType::CHARACTER_SELECTION:
            return new CharacterSelectionMessage();
        default:
            return nullptr;
    }
}