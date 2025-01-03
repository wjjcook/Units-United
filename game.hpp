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

#include "player.hpp"

#include "ui/ui.hpp"
#include "ui/text.hpp"
#include "ui/button.hpp"
#include "ui/title_screen.hpp"
#include "ui/character_select_screen.hpp"
#include "ui/play_screen.hpp"

#include "messages/character_selection_message.hpp"
#include "messages/unit_order_message.hpp"
#include "messages/string_message.hpp"
#include "messages/attack_message.hpp"
#include "messages/passive_event_message.hpp"

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

enum Player_Num {
    PLAYER1 = 1,
    PLAYER2 = 2
};

enum Turn_State {
    selectAction, attackEnemy, specialEnemy, healAlly, specialAlly, endTurn
};

class Game {
    public:
        Game();
        ~Game();

        bool init(const std::string& title, int width, int height);
        void run();
        void unitAttack(Unit* attacker, Unit* victim, int rawDmg, int newDmg);
        void sendPassiveEvents(std::vector<PassiveEventMessage> events);
        void updateUnitUI(Unit* unit);
        void updateUIAfterAttack(Unit* attacker, Unit* victim, int dmg, std::string customAnnouncement = "");
        std::string generateCustomAnnouncement(Unit* victim = nullptr, std::vector<int> multiAttacks = {});

    private:
        bool initializeServer(Uint16 port);
        void closeServer();
        bool connectToServer(const char* serverIP, int port);
        void sendMessage(const Message& msg);
        Message* receiveMessage();

        void initializeColors();
        void initializeTitleElements(SDL_Renderer* renderer);
        void initializeCSelectElements(SDL_Renderer* renderer);
        void updateTimelineText();
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
        void initiateAttackOnClick(std::string targetName);
        void handleEndEvents(SDL_Event e);
        void resetGame();
        
        void update();
        void roundOver();
        std::string receiveAndHandlePassiveMessages(int firstAttack);
        void updateTimeline();
        Unit* findNextUnit(Unit* currentUnit);
        void render();

        IPaddress ip;
        TCPsocket server;
        TCPsocket client;
        SDLNet_SocketSet socketSet;

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
        bool remoteCSelectDone;
        Turn_State turnState;

        std::map<std::string, SDL_Color> colorMap;

        std::string fighterFirstTarget;
        int fighterFirstAttack;

        // UI
        UI* titleScreen;
        UI* cSelectScreen;
        UI* playScreen;

        // Players
        Player* player1;
        Player* player2;
        Player_Num playerTurn;
        Player_Num winner;
        
        // Units
        std::unordered_map<std::string, std::function<Unit*()>> unitCreators;
        
        std::deque<Unit*> gameUnits;
        std::vector<Unit*> unitsById;
        Unit* currentUnit;

};

#endif