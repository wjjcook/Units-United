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
    playerTurn = PLAYER1;

    caveman = nullptr;
    
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
    initializeColors();
    initializeTitleElements(renderer);
    initializeCSelectElements(renderer);

    running = true;
    return true;
}

void Game::initializeColors() {
    colorMap["white"] = {255, 255, 255, 255};
    colorMap["black"] = {0, 0, 0, 255};

    // Need to find proper color palette
    colorMap["red"] = {255, 0, 0, 255};
    colorMap["blue"] = {0, 0, 255, 255};
    colorMap["green"] = {37, 112, 37, 255};
    colorMap["light red"] = {255, 50, 50, 255};
    colorMap["light blue"] = {50, 50, 255, 255};
    colorMap["dark red"] = {139, 27, 7, 255};
    colorMap["dark blue"] = {37, 52, 94, 255};
    colorMap["grey"] = {45, 45, 45, 255};

}

void Game::initializeTitleElements(SDL_Renderer* renderer) {

    titleText = new Text(renderer, "Terminal.ttf", 48);
    titleText->setText("Units United", colorMap["white"]);

    startButton = new Button(renderer, "Terminal.ttf", 24, "Start", colorMap["white"], colorMap["green"], 400, 220, 150, 60);
    startButton->setOutline(true, colorMap["black"]);

    quitButton = new Button(renderer, "Terminal.ttf", 24, "Quit", colorMap["white"], colorMap["dark red"], 400, 340, 150, 60);
    quitButton->setOutline(true, colorMap["black"]);
}

void Game::initializeCSelectElements(SDL_Renderer* renderer) {

    announcerText = new Text(renderer, "Terminal.ttf", 24);

    Text* player1HeaderText = new Text(renderer, "Terminal.ttf", 24);
    player1HeaderText->setText("Player 1 Units:", colorMap["light blue"]);

    Text* player2HeaderText = new Text(renderer, "Terminal.ttf", 24);
    player2HeaderText->setText("Player 2 Units:", colorMap["light red"]);

    Button* blademasterButton = new Button(renderer, "Terminal.ttf", 16, "The Blademaster", colorMap["white"], colorMap["grey"], 25, 50, 150, 50);
    blademasterButton->setOutline(true, colorMap["black"]);

    Button* cavemanButton = new Button(renderer, "Terminal.ttf", 16, "The Caveman", colorMap["white"], colorMap["grey"], 200, 50, 150, 50);
    cavemanButton->setOutline(true, colorMap["black"]);

    Button* duelistButton = new Button(renderer, "Terminal.ttf", 16, "The Duelist", colorMap["white"], colorMap["grey"], 375, 50, 150, 50);
    duelistButton->setOutline(true, colorMap["black"]);

    Button* fighterButton = new Button(renderer, "Terminal.ttf", 16, "The Fighter", colorMap["white"], colorMap["grey"], 550, 50, 150, 50);
    fighterButton->setOutline(true, colorMap["black"]);

    Button* medicButton = new Button(renderer, "Terminal.ttf", 16, "The Medic", colorMap["white"], colorMap["grey"], 25, 150, 150, 50);
    medicButton->setOutline(true, colorMap["black"]);

    Button* raidBossButton = new Button(renderer, "Terminal.ttf", 16, "The Raid Boss", colorMap["white"], colorMap["grey"], 200, 150, 150, 50);
    raidBossButton->setOutline(true, colorMap["black"]);

    Button* tankButton = new Button(renderer, "Terminal.ttf", 16, "The Tank", colorMap["white"], colorMap["grey"], 375, 150, 150, 50);
    tankButton->setOutline(true, colorMap["black"]);

    Button* villageIdiotButton = new Button(renderer, "Terminal.ttf", 16, "The Village Idiot", colorMap["white"], colorMap["grey"], 550, 150, 150, 50);
    villageIdiotButton->setOutline(true, colorMap["black"]);
    
    cSelectUnitButtons.push_back(blademasterButton);
    cSelectUnitButtons.push_back(cavemanButton);
    cSelectUnitButtons.push_back(duelistButton);
    cSelectUnitButtons.push_back(fighterButton);
    cSelectUnitButtons.push_back(medicButton);
    cSelectUnitButtons.push_back(raidBossButton);
    cSelectUnitButtons.push_back(tankButton);
    cSelectUnitButtons.push_back(villageIdiotButton);

    player1SelectText.push_back(player1HeaderText);
    player2SelectText.push_back(player2HeaderText);
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
    int x, y;
    SDL_GetMouseState(&x, &y);
    for (unsigned int i = 0; i < cSelectUnitButtons.size(); i++){
        if (cSelectUnitButtons[i]->isHovered(x, y)) {
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                std::cout << "Clicked!" << std::endl;
            } else {
                cSelectUnitButtons[i]->setHovered(true);
            }
        } else {
            cSelectUnitButtons[i]->setHovered(false);
        }
    }
    
}

void Game::update() {
    // if (playerTurn == PLAYER1) {
    //     announcerText->setText("Player 1: Select a Unit", white);
    // }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 33, 39, 64, 255); // Background color
    SDL_RenderClear(renderer);

    if (gameState == title) {
        titleText->render(325, 100);
        startButton->render();
        quitButton->render();
    } else if (gameState == cSelect) {
        for (unsigned int i = 0; i < cSelectUnitButtons.size(); i++) {
            cSelectUnitButtons[i]->render();
        }
        for (unsigned int i = 0; i < player1SelectText.size(); i++) {
            player1SelectText[i]->render(750, ((i)*35)+50);
        }
        for (unsigned int i = 0; i < player2SelectText.size(); i++) {
            player2SelectText[i]->render(750, ((i)*35)+270);
        }
    }

    SDL_RenderPresent(renderer);
}