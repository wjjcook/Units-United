#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Button {
    public:
        Button(SDL_Renderer* renderer, const std::string& fontPath, int fontSize, const std::string& text, SDL_Color textColor, SDL_Color buttonColor, int x, int y, int width, int height);
        ~Button();

        void render();
        bool isHovered(int mouseX, int mouseY);
        void setOutline(bool enable, SDL_Color color);
        void setHovered(bool hoverStatus);

    private:
        SDL_Renderer* mRenderer;
        TTF_Font* mFont;
        SDL_Texture* mTexture;
        SDL_Color mButtonColor;
        SDL_Color mOutlineColor;
        SDL_Rect mRect;
        bool mDrawOutline;
        bool hovered;

        void createTextTexture(const std::string& text, SDL_Color color);
};

#endif