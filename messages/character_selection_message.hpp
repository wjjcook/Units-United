#ifndef CHARACTER_SELECTION_MESSAGE_H
#define CHARACTER_SELECTION_MESSAGE_H

#include <set>
#include <string>

#include "Message.hpp"

class CharacterSelectionMessage : public Message {
    public:
        CharacterSelectionMessage();
        CharacterSelectionMessage(std::set<std::string> selectedUnits);

        MessageType getType() const override;
        void setUnits(std::set<std::string> selectedUnits);
        void serialize(char* buffer) const override;
        void deserialize(const char* buffer) override;

    private:
        std::set<std::string> units;
};

#endif