#include "game.hpp"
#include <iostream>

Game::Game() : gameWindow(nullptr), renderer(nullptr), running(false), titleText(nullptr) {}

Game::~Game() {
    clean();
}

bool Game::init(const std::string& title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cout << "TTF_Init: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    gameWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!gameWindow) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(gameWindow);
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    titleText = new Text(renderer, "Terminal.ttf", 48);
    SDL_Color white = {255, 255, 255, 255};
    titleText->setText("Units United", white);

    SDL_Color blue = {0, 0, 255, 255};
    SDL_Color black = {0, 0, 0, 255};

    // Initialize button
    startButton = new Button(renderer, "Terminal.ttf", 24, "Start", white, blue, 270, 220, 150, 60);
    startButton->setOutline(true, black);

    running = true;
    return true;
}

void Game::run() {
    while (running) {
        handleEvents();
        update();
        render();
    }
}

void Game::clean() {
    delete titleText;
    delete startButton;
    // delete quitButton;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);
    TTF_Quit();
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            running = false;
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (startButton->isClicked(x, y)) {
                std::cout << "Button clicked!" << std::endl;
            }
        }
    }
}

void Game::update() {
    // Update game state
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 11, 45, 82, 255); // Background color
    SDL_RenderClear(renderer);

    titleText->renderCentered(640, 240);
    startButton->render();

    SDL_RenderPresent(renderer);
}