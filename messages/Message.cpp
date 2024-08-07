#include "message.hpp"

Message* Message::createMessage(MessageType type) {
    switch (type) {
        // case MessageType::ATTACK:
        //     // return new AttackMessage();
        // case MessageType::MOVE:
        //     // return new MoveMessage();
        default:
            return nullptr;
    }
}