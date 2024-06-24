#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <map>

#include "text.hpp"
#include "button.hpp"
#include "player.hpp"
#include "caveman.hpp"

enum Game_State {
    title, cSelect, play, pause, end
};

enum Player_Turn {
    PLAYER1 = 1,
    PLAYER2 = 2
};

class Game {
    public:
        Game();
        ~Game();

        bool init(const std::string& title, int width, int height);
        void run();

    private:
        void initializeColors();
        void initializeTitleElements(SDL_Renderer* renderer);
        void initializeCSelectElements(SDL_Renderer* renderer);
        void handleEvents();
        void handleTitleEvents(SDL_Event e);
        void handleCSelectEvents(SDL_Event e);
        void update();
        void render();

        SDL_Window* gameWindow;
        SDL_Renderer* renderer;
        bool running;
        Game_State gameState;

        std::map<std::string, SDL_Color> colorMap;

        // Players
        Player* player1;
        Player* player2;
        Player_Turn playerTurn;

        // Text objects
        Text* titleText;
        Text* announcerText;
        std::vector<Text*> player1SelectText;
        std::vector<Text*> player2SelectText;

        // Button objects
        Button* startButton;
        Button* quitButton;

        std::vector<Button*> cSelectUnitButtons;

        Caveman* caveman;
};

#endif