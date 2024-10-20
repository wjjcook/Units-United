#ifndef PASSIVE_EVENT_MESSAGE_H
#define PASSIVE_EVENT_H

#include <vector>

#include "Message.hpp"

class PassiveEventMessage : public Message {
    public:
        PassiveEventMessage();
        PassiveEventMessage(std::string unitName, std::string passiveType, int value);

        MessageType getType() const override;
        void serialize(char* buffer) const override;
        void deserialize(const char* buffer) override;

    private:
        std::string unitName;
        std::string passiveType;
        int value;
};

#endif
