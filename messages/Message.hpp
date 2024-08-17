#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstring>
#include <iostream>

enum class MessageType {
    CHARACTER_SELECTION,
    ATTACK,
    MOVE,
};

class Message {
    public:
        virtual ~Message() = default;
        virtual MessageType getType() const = 0;
        virtual void serialize(char* buffer) const = 0;
        virtual void deserialize(const char* buffer) = 0;

        static Message* createMessage(MessageType type);
        MessageType type;

    protected:
        
};

#endif