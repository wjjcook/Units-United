#include "game.hpp"

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 520;

int main(int argc, char* args[]) {
    Game game;

    if (!game.init("Units United", SCREEN_WIDTH, SCREEN_HEIGHT)) {
        return 1;
    }

    game.run();
    game.clean();

    return 0;
}