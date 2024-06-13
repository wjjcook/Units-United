#ifndef GAME_H
#define GAME_H

enum gameState {
    title, cSelect, play, pause, end
};

class Game {
    public:
        void start();
    private:
        gameState gState;

        void setup();
        void mainLoop();
};

#endif