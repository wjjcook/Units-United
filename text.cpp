#include "text.hpp"
#include <iostream>

Text::Text(SDL_Renderer* renderer, const std::string& fontPath, int fontSize, float scaleX, float scaleY)
    : mRenderer(renderer), mFont(nullptr), mTexture(nullptr), mWidth(0), mHeight(0) {
    this->scaleX = scaleX;
    this->scaleY = scaleY;
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
    int scaledFontSize = static_cast<int>(fontSize * std::min(scaleX, scaleY));
    mFont = TTF_OpenFont(fontPath.c_str(), scaledFontSize);
    if (mFont == nullptr) {
        std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

std::string Text::getContent() {
    return content;
}

void Text::setText(const std::string& text, SDL_Color color) {
    content = text;
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
    int scaledX = static_cast<int>(x * scaleX);
    int scaledY = static_cast<int>(y * scaleY);
    if (mTexture != nullptr) {
        SDL_Rect renderQuad = {scaledX , scaledY, mWidth, mHeight };
        SDL_RenderCopy(mRenderer, mTexture, nullptr, &renderQuad);
    }
}
