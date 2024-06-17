#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "text.hpp"
#include "button.hpp"
#include "player.hpp"

enum Game_State {
    title, cSelect, play, pause, end
};

class Game {
public:
    Game();
    ~Game();

    bool init(const std::string& title, int width, int height);
    void run();
    void clean();

private:
    void handleEvents();
    void handleTitleEvents(SDL_Event e);
    void handleCSelectEvents(SDL_Event e);
    void update();
    void render();

    SDL_Window* gameWindow;
    SDL_Renderer* renderer;
    bool running;
    Game_State gameState;

    // Players
    Player* player1;
    Player* player2;

    // Text objects
    Text* titleText;

    // Button objects
    Button* startButton;
    Button* quitButton;
};

#endif