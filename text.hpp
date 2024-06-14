#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Text {
public:
    Text(SDL_Renderer* renderer, const std::string& fontPath, int fontSize);
    ~Text();

    bool loadFont(const std::string& fontPath, int fontSize);
    void setText(const std::string& text, SDL_Color color);
    void render(int x, int y);
    void renderCentered(int screenWidth, int screenHeight);

private:
    SDL_Renderer* mRenderer;
    TTF_Font* mFont;
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};

#endif