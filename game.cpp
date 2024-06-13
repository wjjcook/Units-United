#include "Game.hpp"
#include <iostream>

using namespace std;

void Game::start() {
    setup();
    mainLoop();
}

void Game::setup() {
    cout << "Setup" << endl;
    gState = title;
}

void Game::mainLoop() {
    bool gameRunning = true;
    cout << "Running" << endl;
    while (gameRunning) {
        if (gState == title) {
            
        }
    }
}