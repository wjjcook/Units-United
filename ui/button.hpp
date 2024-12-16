#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Button {
    public:
        Button(SDL_Renderer* renderer, const std::string& fontPath, int fontSize, const std::string& text, SDL_Color textColor, SDL_Color buttonColor, int width, int height, float scaleX, float scaleY);
        ~Button();

        void render(int x, int y);
        bool isHovered(int mouseX, int mouseY);
        void setOutline(bool enable, SDL_Color color);
        void setHovered(bool hoverStatus);
        std::string getText();

    private:
        SDL_Renderer* mRenderer;
        TTF_Font* mFont;
        SDL_Texture* mTexture;
        SDL_Color mButtonColor;
        SDL_Color mOutlineColor;
        SDL_Rect mRect;
        int mWidth;
        int mHeight;
        float scaleX;
        float scaleY;
        bool mDrawOutline;
        bool hovered;
        std::string mText;

        void createTextTexture(const std::string& text, SDL_Color color);
};

#endif