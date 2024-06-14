#include "game.hpp"

int main(int argc, char* args[]) {
    Game game;

    if (!game.init("Units United", 640, 480)) {
        return 1;
    }

    game.run();
    game.clean();

    return 0;
}