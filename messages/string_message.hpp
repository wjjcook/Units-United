#ifndef STRING_MESSAGE_H
#define STRING_MESSAGE_H

#include <vector>
#include <string>

#include "message.hpp"

class StringMessage : public Message {
    public:
        StringMessage();
        StringMessage(std::string s);

        MessageType getType() const override;
        std::string getString();
        void setString(std::string s);
        void serialize(char* buffer) const override;
        void deserialize(const char* buffer) override;

    private:
        std::string str;
};

#endif