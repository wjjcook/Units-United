#ifndef UNIT_ORDER_MESSAGE_H
#define UNIT_ORDER_MESSAGE_H

#include <map>
#include <string>

#include "Message.hpp"

class UnitOrderMessage : public Message {
    public:
        UnitOrderMessage();
        UnitOrderMessage(std::map<std::string, int> selectedUnits);

        MessageType getType() const override;
        std::map<std::string, int> getUnits();
        void setUnits(std::map<std::string, int> selectedUnits);
        void serialize(char* buffer) const override;
        void deserialize(const char* buffer) override;

    private:
        std::map<std::string, int> units;
};

#endif