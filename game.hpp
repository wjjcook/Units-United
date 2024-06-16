#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "text.hpp"
#include "button.hpp"

class Game {
public:
    Game();
    ~Game();

    bool init(const std::string& title, int width, int height);
    void run();
    void clean();

private:
    void handleEvents();
    void update();
    void render();

    SDL_Window* gameWindow;
    SDL_Renderer* renderer;
    bool running;

    // Text objects
    Text* titleText;

    // Button objects
    Button* startButton;
    Button* quitButton;
};

#endif