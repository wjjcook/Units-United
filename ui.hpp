#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <map>

class UI {
    public:
        UI();
        virtual ~UI();

        virtual void initializeElements();

    protected:
        void initializeColors();

        std::map<std::string, SDL_Color> colorMap;
};

#endif