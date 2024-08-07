#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_NET.h>
#include <string>
#include <map>
#include <unordered_map>
#include <functional>
#include <deque>

#include "text.hpp"
#include "button.hpp"
#include "player.hpp"
#include "units/blademaster.hpp"
#include "units/caveman.hpp"
#include "units/duelist.hpp"
#include "units/fighter.hpp"
#include "units/medic.hpp"
#include "units/raid_boss.hpp"
#include "units/tank.hpp"
#include "units/village_idiot.hpp"

enum Game_State {
    title, cSelect, play, pause, end
};

enum Player_Turn {
    PLAYER1 = 1,
    PLAYER2 = 2
};

enum Turn_State {
    selectAction, selectEnemy, selectAlly, endTurn
};

class Game {
    public:
        Game();
        ~Game();

        bool init(const std::string& title, int width, int height);
        void run();

    private:
        bool initializeServer(Uint16 port);
        void closeServer();
        bool connectToServer(const char* serverIP, int port);
        void initializeColors();
        void initializeTitleElements(SDL_Renderer* renderer);
        void initializeCSelectElements(SDL_Renderer* renderer);
        void populateUnitMap();
        Unit* createUnit(const std::string& unitName);

        void handleEvents();
        int checkMouseEvent(Button* button, SDL_Event e);
        void handleTitleEvents(SDL_Event e);
        void handleCSelectEvents(SDL_Event e);
        void addUnitToRoster(std::string unit);
        void initializeMatch();
        void populateUnitButtonMap();
        void handlePlayEvents(SDL_Event e);
        
        void update();
        Unit* findNextUnit(Unit* currentUnit);
        void render();

        IPaddress ip;
        TCPsocket server;
        TCPsocket client;
        SDL_Window* gameWindow;
        SDL_Renderer* renderer;
        int screenWidth;
        int screenHeight;
        float scaleX;
        float scaleY;

        bool running;
        bool searchForClient;
        bool receiveIpInput;
        std::string ipInput;
        bool ipInputDone;
        Game_State gameState;
        bool cSelectDone;
        Turn_State turnState;

        std::map<std::string, SDL_Color> colorMap;

        // Players
        Player* player1;
        Player* player2;
        Player_Turn playerTurn;

        // Text objects
        Text* titleText;
        Text* ipText;
        Text* playerTurnText;
        Text* announcerText;
        Text* timelineHeader;
        Text* manaText;
        Text* tempText;
        std::vector<Text*> player1SelectText;
        std::vector<Text*> player2SelectText;
        std::deque<Text*> timeline;
        std::vector<Text*> playUnitTexts;
        std::string inputText;

        // Button objects
        Button* titleStartButton;
        Button* titleJoinButton;
        Button* titleCancelButton;
        Button* quitButton;
        Button* cSelectStartButton;
        std::vector<Button*> cSelectUnitButtons;
        std::vector<Button*> playUnitButtons;
        std::unordered_map<std::string, std::vector<Button*>> unitButtonMap;
        
        // Units
        std::unordered_map<std::string, std::function<Unit*()>> unitCreators;
        
        std::deque<Unit*> gameUnits;
        Unit* currentUnit;


};

#endif