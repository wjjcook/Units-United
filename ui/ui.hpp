#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <map>

#include "../text.hpp"
#include "../button.hpp"

enum class TextType {
    Title,
    Announcer,
    IP
};
enum class ButtonType {
    Start,
    Join,
    Quit
};

class UI {
    public:
        UI(SDL_Renderer* renderer, float scaleX, float scaleY);
        virtual ~UI() = default;

        virtual void initializeElements() = 0;

        virtual Text* getText(TextType type) = 0;
        virtual void setText(TextType type, const std::string& newText) = 0;
        virtual Button* getButton(ButtonType type) = 0;
        virtual void render() = 0;

    protected:
        void initializeColors();

        SDL_Renderer* renderer;
        float scaleX;
        float scaleY;
        std::map<std::string, SDL_Color> colorMap;
};

#endif