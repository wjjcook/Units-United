#include "game.hpp"
#include <iostream>

Game::Game() {
    gameWindow = nullptr;
    renderer = nullptr;
    running = false;
    gameState = title;
    cSelectDone = false;
    endTurn = false;

    titleText = nullptr;
    announcerText = nullptr;
    timelineHeader = nullptr;

    titleStartButton = nullptr;
    quitButton = nullptr;
    cSelectStartButton = nullptr;

    tempText = nullptr;

    player1 = nullptr;
    player2 = nullptr;
    playerTurn = PLAYER1;

    currentUnit = nullptr;
    
}

Game::~Game() {
    delete titleText;
    delete announcerText;
    delete timelineHeader;
    delete tempText;

    for (unsigned int i = 0; i < player1SelectText.size(); i++) {
        delete player1SelectText[i];
    }

    for (unsigned int i = 0; i < player2SelectText.size(); i++) {
        delete player2SelectText[i];
    }

    delete titleStartButton;
    delete quitButton;
    delete cSelectStartButton;

    delete player1;
    delete player2;
    
    for (unsigned int i = 0; i < cSelectUnitButtons.size(); i++) {
        delete cSelectUnitButtons[i];
    }

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

    // SDL_DisplayMode displayMode;
    // if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
    //     std::cerr << "Could not get display mode for video display 0: " << SDL_GetError() << std::endl;
    //     SDL_Quit();
    //     return -1;
    // }

    // int screenWidth = displayMode.w - 320;
    // int screenHeight = displayMode.h - 200;

    gameWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN); // Replace width/height with screenWidth/screenHeight eventually
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
    populateUnitMap();

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
    colorMap["light blue"] = {3, 177, 252, 255};
    colorMap["dark red"] = {139, 27, 7, 255};
    colorMap["dark blue"] = {26, 49, 112, 255};
    colorMap["grey"] = {45, 45, 45, 255};
}

void Game::initializeTitleElements(SDL_Renderer* renderer) {

    titleText = new Text(renderer, "Terminal.ttf", 48);
    titleText->setText("Units United", colorMap["white"]);

    titleStartButton = new Button(renderer, "Terminal.ttf", 24, "Start", colorMap["white"], colorMap["green"], 400, 220, 150, 60);
    titleStartButton->setOutline(true, colorMap["black"]);

    quitButton = new Button(renderer, "Terminal.ttf", 24, "Quit", colorMap["white"], colorMap["dark red"], 400, 340, 150, 60);
    quitButton->setOutline(true, colorMap["black"]);
}

void Game::initializeCSelectElements(SDL_Renderer* renderer) {

    announcerText = new Text(renderer, "Terminal.ttf", 24);

    tempText = new Text(renderer, "Terminal.ttf", 48);
    tempText->setText("NEW UNITS COMING SOON...", colorMap["white"]);

    cSelectStartButton = new Button(renderer, "Terminal.ttf", 24, "Start Game", colorMap["white"], colorMap["green"], 475, 430, 150, 60);
    cSelectStartButton->setOutline(true, colorMap["black"]);

    Text* player1HeaderText = new Text(renderer, "Terminal.ttf", 24);
    player1HeaderText->setText("Player 1 Units:", colorMap["light blue"]);

    Text* player2HeaderText = new Text(renderer, "Terminal.ttf", 24);
    player2HeaderText->setText("Player 2 Units:", colorMap["light red"]);

    player1SelectText.push_back(player1HeaderText);
    player2SelectText.push_back(player2HeaderText);

    Button* blademasterButton = new Button(renderer, "Terminal.ttf", 16, "The Blademaster", colorMap["white"], colorMap["grey"], 25, 50, 150, 60);
    blademasterButton->setOutline(true, colorMap["black"]);

    Button* cavemanButton = new Button(renderer, "Terminal.ttf", 16, "The Caveman", colorMap["white"], colorMap["grey"], 200, 50, 150, 60);
    cavemanButton->setOutline(true, colorMap["black"]);

    Button* duelistButton = new Button(renderer, "Terminal.ttf", 16, "The Duelist", colorMap["white"], colorMap["grey"], 375, 50, 150, 60);
    duelistButton->setOutline(true, colorMap["black"]);

    Button* fighterButton = new Button(renderer, "Terminal.ttf", 16, "The Fighter", colorMap["white"], colorMap["grey"], 550, 50, 150, 60);
    fighterButton->setOutline(true, colorMap["black"]);

    Button* medicButton = new Button(renderer, "Terminal.ttf", 16, "The Medic", colorMap["white"], colorMap["grey"], 25, 150, 150, 60);
    medicButton->setOutline(true, colorMap["black"]);

    Button* raidBossButton = new Button(renderer, "Terminal.ttf", 16, "The Raid Boss", colorMap["white"], colorMap["grey"], 200, 150, 150, 60);
    raidBossButton->setOutline(true, colorMap["black"]);

    Button* tankButton = new Button(renderer, "Terminal.ttf", 16, "The Tank", colorMap["white"], colorMap["grey"], 375, 150, 150, 60);
    tankButton->setOutline(true, colorMap["black"]);

    Button* villageIdiotButton = new Button(renderer, "Terminal.ttf", 16, "The Village Idiot", colorMap["white"], colorMap["grey"], 550, 150, 150, 60);
    villageIdiotButton->setOutline(true, colorMap["black"]);
    
    cSelectUnitButtons.push_back(blademasterButton);
    cSelectUnitButtons.push_back(cavemanButton);
    cSelectUnitButtons.push_back(duelistButton);
    cSelectUnitButtons.push_back(fighterButton);
    cSelectUnitButtons.push_back(medicButton);
    cSelectUnitButtons.push_back(raidBossButton);
    cSelectUnitButtons.push_back(tankButton);
    cSelectUnitButtons.push_back(villageIdiotButton);

}

void Game::populateUnitMap() {
    unitCreators["The Blademaster"] = []() -> Unit* {
        return new Blademaster();
    };
    unitCreators["The Caveman"] = []() -> Unit* {
        return new Caveman();
    };
    unitCreators["The Duelist"] = []() -> Unit* {
        return new Duelist();
    };
    unitCreators["The Fighter"] = []() -> Unit* {
        return new Fighter();
    };
    unitCreators["The Medic"] = []() -> Unit* {
        return new Medic();
    };
    unitCreators["The Raid Boss"] = []() -> Unit* {
        return new RaidBoss();
    };
    unitCreators["The Tank"] = []() -> Unit* {
        return new Tank();
    };
    unitCreators["The Village Idiot"] = []() -> Unit* {
        return new VillageIdiot();
    };
}

Unit* Game::createUnit(const std::string& unitName) {
        auto it = unitCreators.find(unitName);
        if (it != unitCreators.end()) {
            return it->second();
        }
        return nullptr;
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
    if (titleStartButton->isHovered(x, y)) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            std::cout << "Going to character selection" << std::endl;
            // Need to create goToCSelect func
            gameState = cSelect;
            player1 = new Player();
            player2 = new Player();
        } else {
            titleStartButton->setHovered(true);
        }
    } else {
        titleStartButton->setHovered(false);
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
    if (!cSelectDone) {
        for (unsigned int i = 0; i < cSelectUnitButtons.size(); i++){
            if (cSelectUnitButtons[i]->isHovered(x, y)) {
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    addUnitToRoster(cSelectUnitButtons[i]->getText());
                } else {
                    cSelectUnitButtons[i]->setHovered(true);
                }
            } else {
                cSelectUnitButtons[i]->setHovered(false);
            }
        }
    } else {
        if (cSelectStartButton->isHovered(x, y)) {
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                gameState = play;
                initializeMatch();
            } else {
                cSelectStartButton->setHovered(true);
            }
        } else {
            cSelectStartButton->setHovered(false);
        }
    }
}

void Game::addUnitToRoster(std::string unit) {
    if (playerTurn == PLAYER1) {
        if (player1->hasUnit(unit)) {
            std::cout << "Unit already in roster!" << std::endl;
            return;
        }
        player1->addUnit(createUnit(unit), 1);

        Text* unitText = new Text(renderer, "Terminal.ttf", 24);
        unitText->setText(unit, colorMap["light blue"]);
        player1SelectText.push_back(unitText);

        playerTurn = PLAYER2;

    } else {
        if (player2->hasUnit(unit)) {
            std::cout << "Unit already in roster!" << std::endl;
            return;
        }
        player2->addUnit(createUnit(unit), 2);
        
        Text* unitText = new Text(renderer, "Terminal.ttf", 24);
        unitText->setText(unit, colorMap["light red"]);
        player2SelectText.push_back(unitText);

        playerTurn = PLAYER1;
    }
}

void Game::initializeMatch() {
    player1->sortUnitsBySpeed();
    player2->sortUnitsBySpeed();
    int i = 0;
    int j = 0;
    while (i < 4 && j < 4) {
        if (player1->getUnits()[i]->getSpeed() > player2->getUnits()[j]->getSpeed()) {
            gameUnits.push_back(player1->getUnits()[i]);
            i++;
        } else if (player1->getUnits()[i]->getSpeed() < player2->getUnits()[j]->getSpeed()) {
            gameUnits.push_back(player2->getUnits()[j]);
            j++;
        } else {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(0, 1);
            int randomChoice = dist(gen);
            if (randomChoice == 0) {
                gameUnits.push_back(player1->getUnits()[i]);
                i++;
            } else {
                gameUnits.push_back(player2->getUnits()[j]);
                j++;
            }
        }
    }
    while (i < 4) {
        gameUnits.push_back(player1->getUnits()[i]);
        i++;
    }
    while (j < 4) {
        gameUnits.push_back(player2->getUnits()[j]);
        j++;
    }

    timelineHeader = new Text(renderer, "Terminal.ttf", 32);
    timelineHeader->setText("Timeline", colorMap["white"]);
    
    for (unsigned int i = 0; i < gameUnits.size(); i++) {
        Text* unitText = new Text(renderer, "Terminal.ttf", 24);
        if (gameUnits[i]->getPlayerNum() == 1) {
            unitText->setText("P1: " + gameUnits[i]->getName(), colorMap["light blue"]);
        } else {
            unitText->setText("P2: " + gameUnits[i]->getName(), colorMap["light red"]);
        }
        
        timeline.push_back(unitText);
    }

    for (unsigned int i = 0; i < player1->getUnits().size(); i++) {
        Button* newButton = new Button(renderer, "Terminal.ttf", 12, player1->getUnits()[i]->getName(), colorMap["white"], colorMap["dark blue"], (i*150)+25, 50, 120, 40);
        newButton->setOutline(true, colorMap["black"]);
        playUnitButtons.push_back(newButton);

        Text* newHpText = new Text(renderer, "Terminal.ttf", 12);
        int currentHp = player1->getUnits()[i]->getCurrHp();
        int maxHp = player1->getUnits()[i]->getMaxHp();
        std::string hpString = "HP: " + std::to_string(currentHp) + "/" + std::to_string(maxHp);
        newHpText->setText(hpString, colorMap["white"]);
        playUnitTexts.push_back(newHpText);
    }
    for (unsigned int i = 0; i < player2->getUnits().size(); i++) {
        Button* newButton = new Button(renderer, "Terminal.ttf", 12, player1->getUnits()[i]->getName(), colorMap["white"], colorMap["dark red"], (i*150)+25, 200, 120, 40);
        newButton->setOutline(true, colorMap["black"]);
        playUnitButtons.push_back(newButton);

        Text* newHpText = new Text(renderer, "Terminal.ttf", 12);
        int currentHp = player2->getUnits()[i]->getCurrHp();
        int maxHp = player2->getUnits()[i]->getMaxHp();
        std::string hpString = "HP: " + std::to_string(currentHp) + "/" + std::to_string(maxHp);
        newHpText->setText(hpString, colorMap["white"]);
        playUnitTexts.push_back(newHpText);
    }

    endTurn = false;
}

void Game::update() {
    if (gameState == cSelect) {
        if (player1->getUnits().size() >= 4 && player2->getUnits().size() >= 4) {
            cSelectDone = true;
            announcerText->setText("All units selected, ready to start!", colorMap["white"]);
            for (unsigned int i = 0; i < cSelectUnitButtons.size(); i++){
                cSelectUnitButtons[i]->setHovered(false);
            }
        } else {
            if (playerTurn == PLAYER1) {
                announcerText->setText("Player 1: Select a Unit", colorMap["light blue"]);
            } else {
                announcerText->setText("Player 2: Select a Unit", colorMap["light red"]);
            }
        }
    } else if (gameState == play) {
        if (currentUnit == nullptr) {
            currentUnit = gameUnits.front();
        } else if (endTurn) {
            currentUnit = findNextUnit(currentUnit);
        }
        if (currentUnit->getPlayerNum() == 1) {
            playerTurn = PLAYER1;
        } else {
            playerTurn = PLAYER2;
        }

        if (playerTurn == PLAYER1) {
            announcerText->setText("Player 1's Turn: " + currentUnit->getName(), colorMap["light blue"]);
        } else {
            announcerText->setText("Player 2's Turn: " + currentUnit->getName(), colorMap["light red"]);
        }
    }
    
}

Unit* Game::findNextUnit(Unit* currentUnit) {
    auto it = std::find(gameUnits.begin(), gameUnits.end(), currentUnit);

    if (it != gameUnits.end()) {
        ++it;
        if (it != gameUnits.end()) {
            return *it;
        } else {
            return gameUnits.front();
        }
    }
    return gameUnits.front();
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 33, 39, 64, 255); // Background color
    SDL_RenderClear(renderer);

    if (gameState == title) {
        titleText->render(325, 100);
        titleStartButton->render();
        quitButton->render();
    } else if (gameState == cSelect) {
        announcerText->render(25, 450);
        tempText->render(50, 300);
        cSelectStartButton->render();
        for (unsigned int i = 0; i < cSelectUnitButtons.size(); i++) {
            cSelectUnitButtons[i]->render();
        }
        for (unsigned int i = 0; i < player1SelectText.size(); i++) {
            player1SelectText[i]->render(725, ((i)*35)+50);
        }
        for (unsigned int i = 0; i < player2SelectText.size(); i++) {
            player2SelectText[i]->render(725, ((i)*35)+270);
        }
    } else if (gameState == play) {
        announcerText->render(25, 365);
        timelineHeader->render(725, 50);
        for (unsigned int i = 0; i < timeline.size(); i++) {
            timeline[i]->render(675, ((i+2)*35)+50);
        }
        for (unsigned int i = 0; i < 8; i++) {
            playUnitButtons[i]->render();
            if (i < 4) {
                playUnitTexts[i]->render((i*150)+27, 100);
            } else {
                playUnitTexts[i]->render(((i-4)*150)+27, 250);
            }
            
        }
        
    }

    SDL_RenderPresent(renderer);
}