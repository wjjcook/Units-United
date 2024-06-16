#include "text.hpp"
#include <iostream>

Text::Text(SDL_Renderer* renderer, const std::string& fontPath, int fontSize)
    : mRenderer(renderer), mFont(nullptr), mTexture(nullptr), mWidth(0), mHeight(0) {
    loadFont(fontPath, fontSize);
}

Text::~Text() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
    }
    if (mFont != nullptr) {
        TTF_CloseFont(mFont);
    }
}

bool Text::loadFont(const std::string& fontPath, int fontSize) {
    mFont = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (mFont == nullptr) {
        std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void Text::setText(const std::string& text, SDL_Color color) {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, text.c_str(), color);
    if (textSurface == nullptr) {
        std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    mTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
    if (mTexture == nullptr) {
        std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
    } else {
        mWidth = textSurface->w;
        mHeight = textSurface->h;
    }

    SDL_FreeSurface(textSurface);
}

void Text::render(int x, int y) {
    if (mTexture != nullptr) {
        SDL_Rect renderQuad = { x, y, mWidth, mHeight };
        SDL_RenderCopy(mRenderer, mTexture, nullptr, &renderQuad);
    }
}
