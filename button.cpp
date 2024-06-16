#include "button.hpp"
#include <iostream>

Button::Button(SDL_Renderer* renderer, const std::string& fontPath, int fontSize, const std::string& text, SDL_Color textColor, SDL_Color buttonColor, int x, int y, int width, int height)
    : mRenderer(renderer), mFont(nullptr), mTexture(nullptr), mButtonColor(buttonColor), mRect({x, y, width, height}), mDrawOutline(false) {
    mFont = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (mFont == nullptr) {
        std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
    } else {
        createTextTexture(text, textColor);
    }
}

Button::~Button() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
    }
    if (mFont != nullptr) {
        TTF_CloseFont(mFont);
    }
}

void Button::createTextTexture(const std::string& text, SDL_Color color) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, text.c_str(), color);
    if (textSurface == nullptr) {
        std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
    } else {
        mTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
        if (mTexture == nullptr) {
            std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(textSurface);
    }
}

void Button::render() {
    // Render button background
    if (hovered) {
        SDL_SetRenderDrawColor(mRenderer, mButtonColor.r, mButtonColor.g + 40, mButtonColor.b + 40, mButtonColor.a);
    } else {
        SDL_SetRenderDrawColor(mRenderer, mButtonColor.r, mButtonColor.g, mButtonColor.b, mButtonColor.a);
    }
    SDL_RenderFillRect(mRenderer, &mRect);

    // Render button text
    if (mTexture != nullptr) {
        int textWidth, textHeight;
        SDL_QueryTexture(mTexture, nullptr, nullptr, &textWidth, &textHeight);
        SDL_Rect textRect = {mRect.x + (mRect.w - textWidth) / 2, mRect.y + (mRect.h - textHeight) / 2, textWidth, textHeight};
        SDL_RenderCopy(mRenderer, mTexture, nullptr, &textRect);
    }

    // Render button outline if enabled
    if (mDrawOutline) {
        SDL_SetRenderDrawColor(mRenderer, mOutlineColor.r, mOutlineColor.g, mOutlineColor.b, mOutlineColor.a);
        SDL_RenderDrawRect(mRenderer, &mRect);
    }
}

bool Button::isHovered(int mouseX, int mouseY) {
    return mouseX >= mRect.x && mouseX <= (mRect.x + mRect.w) && mouseY >= mRect.y && mouseY <= (mRect.y + mRect.h);
}

void Button::setOutline(bool enable, SDL_Color color) {
    mDrawOutline = enable;
    mOutlineColor = color;
}

void Button::setHovered(bool hoverStatus) {
    hovered = hoverStatus;
}