#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <map>

#include "ui.hpp"
#include "text.hpp"
#include "button.hpp"

class TitleScreen : public UI {
    public:
        TitleScreen(SDL_Renderer* renderer, float scaleX, float scaleY);
        ~TitleScreen() override;

        void initializeElements() override;

        void setText(TextType type, std::string newText) override;
        Button* getButton(ButtonType type) override;
        void render() override;

    private:
        Text* titleText;
        Text* announcerText;
        Text* ipText;

        Button* startButton;
        Button* joinButton;
        Button* quitButton;
};

#endif