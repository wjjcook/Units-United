#ifndef UNIT_ORDER_MESSAGE_H
#define UNIT_ORDER_MESSAGE_H

#include <vector>
#include <string>

#include "Message.hpp"

class UnitOrderMessage : public Message {
    public:
        UnitOrderMessage();
        UnitOrderMessage(std::vector<std::pair<std::string, int>> selectedUnits);

        MessageType getType() const override;
        std::vector<std::pair<std::string, int>> getUnits();
        void setUnits(std::vector<std::pair<std::string, int>> selectedUnits);
        void serialize(char* buffer) const override;
        void deserialize(const char* buffer) override;

    private:
        std::vector<std::pair<std::string, int>> units;
};

#endif