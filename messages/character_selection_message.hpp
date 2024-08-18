#ifndef CHARACTER_SELECTION_MESSAGE_H
#define CHARACTER_SELECTION_MESSAGE_H

#include <vector>
#include <string>

#include "Message.hpp"

class CharacterSelectionMessage : public Message {
    public:
        CharacterSelectionMessage();
        CharacterSelectionMessage(std::vector<std::string> selectedUnits);

        MessageType getType() const override;
        std::vector<std::string> getUnits();
        void setUnits(std::vector<std::string> selectedUnits);
        void serialize(char* buffer) const override;
        void deserialize(const char* buffer) override;

    private:
        std::vector<std::string> units;
};

#endif