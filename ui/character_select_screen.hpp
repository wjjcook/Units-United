#ifndef CHARACTER_SELECT_SCREEN_H
#define CHARACTER_SELECT_SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <map>
#include <vector>

#include "ui.hpp"
#include "../text.hpp"
#include "../button.hpp"

class CharacterSelectScreen : public UI {
    public:
        CharacterSelectScreen(SDL_Renderer* renderer, float scaleX, float scaleY);
        ~CharacterSelectScreen() override;

        void initializeElements() override;
        void setLocalPlayer(int playerNum);

        void setText(TextType type, const std::string& newText) override;
        void addUnitToPlayerSelectText(std::string unit, int playerNum);
        Button* getButton(ButtonType type) override;
        std::vector<Button*> getUnitButtons();
        void render() override;

    private:
        int localPlayer;
        Text* announcerText;
        Text* tempText;
        std::vector<Text*> player1SelectText;
        std::vector<Text*> player2SelectText;

        Button* startButton;
        std::vector<Button*> unitButtons;
};

#endif