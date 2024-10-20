#include "message.hpp"
#include "character_selection_message.hpp"
#include "unit_order_message.hpp"
#include "string_message.hpp"
#include "attack_message.hpp"
#include "passive_event_message.hpp"

Message* Message::createMessage(MessageType type) {
    switch (type) {
        case MessageType::CHARACTER_SELECTION:
            return new CharacterSelectionMessage();
        case MessageType::UNIT_ORDER:
            return new UnitOrderMessage();
        case MessageType::STRING:
            return new StringMessage();
        case MessageType::ATTACK:
            return new AttackMessage();
        case MessageType::PASSIVE_EVENT:
            return new PassiveEventMessage();
        default:
            return nullptr;
    }
}