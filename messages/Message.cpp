#include "message.hpp"
#include "character_selection_message.hpp"
#include "unit_order_message.hpp"
#include "string_message.hpp"

Message* Message::createMessage(MessageType type) {
    switch (type) {
        case MessageType::CHARACTER_SELECTION:
            return new CharacterSelectionMessage();
        case MessageType::UNIT_ORDER:
            return new UnitOrderMessage();
        case MessageType::STRING:
            return new StringMessage();
        default:
            return nullptr;
    }
}