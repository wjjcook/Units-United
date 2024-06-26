#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <map>
#include <unordered_map>
#include <functional>
#include <deque>

#include "text.hpp"
#include "button.hpp"
#include "player.hpp"
#include "blademaster.hpp"
#include "caveman.hpp"
#include "duelist.hpp"
#include "fighter.hpp"
#include "medic.hpp"
#include "raid_boss.hpp"
#include "tank.hpp"
#include "village_idiot.hpp"

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
        void populateUnitMap();
        Unit* createUnit(const std::string& unitName);
        void handleEvents();
        void handleTitleEvents(SDL_Event e);
        void handleCSelectEvents(SDL_Event e);
        void addUnitToRoster(std::string unit);
        void initializeMatch();
        void update();
        void render();

        SDL_Window* gameWindow;
        SDL_Renderer* renderer;
        bool running;
        Game_State gameState;
        bool cSelectDone;

        std::map<std::string, SDL_Color> colorMap;

        // Players
        Player* player1;
        Player* player2;
        Player_Turn playerTurn;

        // Text objects
        Text* titleText;
        Text* announcerText;
        Text* tempText;
        std::vector<Text*> player1SelectText;
        std::vector<Text*> player2SelectText;
        Text* timelineHeader;
        std::deque<Text*> timeline;

        // Button objects
        Button* titleStartButton;
        Button* quitButton;
        Button* cSelectStartButton;

        std::vector<Button*> cSelectUnitButtons;

        std::unordered_map<std::string, std::function<Unit*()>> unitCreators;

        std::deque<Unit*> gameUnits;

};

#endif