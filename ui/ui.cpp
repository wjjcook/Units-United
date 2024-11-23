#include "ui.hpp"

UI::UI(SDL_Renderer* renderer, float scaleX, float scaleY) {
    this->renderer = renderer;
    this->scaleX = scaleX;
    this->scaleY = scaleY;

    initializeColors();
}

void UI::initializeColors() {
    colorMap["white"] = {255, 255, 255, 255};
    colorMap["black"] = {0, 0, 0, 255};

    // Need to find proper color palette
    colorMap["red"] = {255, 0, 0, 255};
    colorMap["blue"] = {0, 0, 255, 255};
    colorMap["green"] = {37, 112, 37, 255};
    colorMap["light red"] = {255, 50, 50, 255};
    colorMap["light blue"] = {3, 177, 252, 255};
    colorMap["dark red"] = {139, 27, 7, 255};
    colorMap["dark blue"] = {26, 49, 112, 255};
    colorMap["grey"] = {45, 45, 45, 255};
}