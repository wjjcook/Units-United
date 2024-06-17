#include "game.hpp"

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 520;

int main(int argc, char* args[]) {
    Game* game = new Game();

    if (!game->init("Units United", SCREEN_WIDTH, SCREEN_HEIGHT)) {
        return 1;
    }

    game->run();
    
    delete game;

    return 0;
}