#include "game.hpp"
#include <iostream>

Game::Game() {
    gameWindow = nullptr;
    renderer = nullptr;
    running = false;
    gameState = title;
    titleText = nullptr;
    announcerText = nullptr;
    startButton = nullptr;
    quitButton = nullptr;
    player1 = nullptr;
    player2 = nullptr;

}

Game::~Game() {
    delete titleText;
    delete startButton;
    delete quitButton;
    delete player1;
    delete player2;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);
    TTF_Quit();
    SDL_Quit();
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

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color buttonColor = {139, 27, 7, 255};
    SDL_Color black = {0, 0, 0, 255};

    titleText = new Text(renderer, "Terminal.ttf", 48);
    titleText->setText("Units United", white);

    // Initialize buttons
    startButton = new Button(renderer, "Terminal.ttf", 24, "Start", white, buttonColor, 400, 220, 150, 60);
    startButton->setOutline(true, black);

    quitButton = new Button(renderer, "Terminal.ttf", 24, "Quit", white, buttonColor, 400, 340, 150, 60);
    quitButton->setOutline(true, black);

    Text* player1HeaderText = new Text(renderer, "Terminal.ttf", 24);
    player1HeaderText->setText("Player 1 Units:", white);

    Text* player2HeaderText = new Text(renderer, "Terminal.ttf", 24);
    player2HeaderText->setText("Player 2 Units:", white);

    player1SelectText.push_back(player1HeaderText);
    player2SelectText.push_back(player2HeaderText);

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

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            running = false;
        } else {
            if (gameState == title) {
                handleTitleEvents(e);
            } else if (gameState == cSelect) {
                handleCSelectEvents(e);
            }
            
        }
    }
}

void Game::handleTitleEvents(SDL_Event e) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (startButton->isHovered(x, y)) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            std::cout << "Going to character selection" << std::endl;
            // Need to create goToCSelect func
            gameState = cSelect;
            player1 = new Player();
            player2 = new Player();
        } else {
            startButton->setHovered(true);
        }
    } else {
        startButton->setHovered(false);
    }
    if (quitButton->isHovered(x, y)) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            running = false;
        } else {
            quitButton->setHovered(true);
        }
    } else {
        quitButton->setHovered(false);
    }
}

void Game::handleCSelectEvents(SDL_Event e) {
    
}

void Game::update() {
    
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 11, 45, 82, 255); // Background color
    SDL_RenderClear(renderer);

    if (gameState == title) {
        titleText->render(325, 100);
        startButton->render();
        quitButton->render();
    } else if (gameState == cSelect) {
        for (unsigned int i = 0; i < player1SelectText.size(); i++) {
            player1SelectText[i]->render(750, ((i)*35)+50);
        }
        for (unsigned int i = 0; i < player2SelectText.size(); i++) {
            player2SelectText[i]->render(750, ((i)*35)+270);
        }
    }

    SDL_RenderPresent(renderer);
}