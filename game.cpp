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
    playerTurnText = nullptr;
    announcerText = nullptr;
    timelineHeader = nullptr;
    manaText = nullptr;
    tempText = nullptr;

    titleStartButton = nullptr;
    quitButton = nullptr;
    cSelectStartButton = nullptr;

    player1 = nullptr;
    player2 = nullptr;
    playerTurn = PLAYER1;

    currentUnit = nullptr;
    
}

Game::~Game() {

    // Text objects
    delete titleText;
    delete playerTurnText;
    delete announcerText;
    delete timelineHeader;
    delete manaText;
    delete tempText;

    for (unsigned int i = 0; i < player1SelectText.size(); i++) {
        delete player1SelectText[i];
    }

    for (unsigned int i = 0; i < player2SelectText.size(); i++) {
        delete player2SelectText[i];
    }

    for (unsigned int i = 0; i < timeline.size(); i++) {
        delete timeline[i];
    }

    for (unsigned int i = 0; i < playUnitTexts.size(); i++) {
        delete playUnitTexts[i];
    }

    // Button objects
    delete titleStartButton;
    delete quitButton;
    delete cSelectStartButton;
    
    for (unsigned int i = 0; i < cSelectUnitButtons.size(); i++) {
        delete cSelectUnitButtons[i];
    }

    for (unsigned int i = 0; i < playUnitButtons.size(); i++) {
        delete playUnitButtons[i];
    }

    // Player objects
    delete player1;
    delete player2;

    // SDL clean up
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

    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
        std::cerr << "Could not get display mode for video display 0: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    screenWidth = displayMode.w - 320;
    screenHeight = displayMode.h - 200;

    scaleX = static_cast<float>(screenWidth) / 960;
    scaleY = static_cast<float>(screenHeight) / 520;

    gameWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
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

    titleText = new Text(renderer, "Terminal.ttf", 48, scaleX, scaleY);
    titleText->setText("Units United", colorMap["white"]);

    titleStartButton = new Button(renderer, "Terminal.ttf", 24, "Start", colorMap["white"], colorMap["green"], 150, 60, scaleX, scaleY);
    titleStartButton->setOutline(true, colorMap["black"]);

    quitButton = new Button(renderer, "Terminal.ttf", 24, "Quit", colorMap["white"], colorMap["dark red"], 150, 60, scaleX, scaleY);
    quitButton->setOutline(true, colorMap["black"]);
}

void Game::initializeCSelectElements(SDL_Renderer* renderer) {

    announcerText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);

    tempText = new Text(renderer, "Terminal.ttf", 48, scaleX, scaleY);
    tempText->setText("NEW UNITS COMING SOON...", colorMap["white"]);

    cSelectStartButton = new Button(renderer, "Terminal.ttf", 24, "Start Game", colorMap["white"], colorMap["green"], 150, 60, scaleX, scaleY);
    cSelectStartButton->setOutline(true, colorMap["black"]);

    Text* player1HeaderText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
    player1HeaderText->setText("Player 1 Units:", colorMap["light blue"]);

    Text* player2HeaderText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
    player2HeaderText->setText("Player 2 Units:", colorMap["light red"]);

    player1SelectText.push_back(player1HeaderText);
    player2SelectText.push_back(player2HeaderText);

    Button* blademasterButton = new Button(renderer, "Terminal.ttf", 16, "The Blademaster", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
    blademasterButton->setOutline(true, colorMap["black"]);

    Button* cavemanButton = new Button(renderer, "Terminal.ttf", 16, "The Caveman", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
    cavemanButton->setOutline(true, colorMap["black"]);

    Button* duelistButton = new Button(renderer, "Terminal.ttf", 16, "The Duelist", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
    duelistButton->setOutline(true, colorMap["black"]);

    Button* fighterButton = new Button(renderer, "Terminal.ttf", 16, "The Fighter", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
    fighterButton->setOutline(true, colorMap["black"]);

    Button* medicButton = new Button(renderer, "Terminal.ttf", 16, "The Medic", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
    medicButton->setOutline(true, colorMap["black"]);

    Button* raidBossButton = new Button(renderer, "Terminal.ttf", 16, "The Raid Boss", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
    raidBossButton->setOutline(true, colorMap["black"]);

    Button* tankButton = new Button(renderer, "Terminal.ttf", 16, "The Tank", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
    tankButton->setOutline(true, colorMap["black"]);

    Button* villageIdiotButton = new Button(renderer, "Terminal.ttf", 16, "The Village Idiot", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
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
            } else if (gameState == play) {
                handlePlayEvents(e);
            }
        }
    }
}

int Game::checkMouseEvent(Button* button, SDL_Event e) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (button->isHovered(x, y)) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            return 1;
        } else {
            button->setHovered(true);
        }
    } else {
        button->setHovered(false);
    }
    return 0;
}

void Game::handleTitleEvents(SDL_Event e) {
    if (checkMouseEvent(titleStartButton, e) == 1) {
        gameState = cSelect;
        player1 = new Player();
        player2 = new Player();
    }
    if (checkMouseEvent(quitButton, e) == 1) {
        running = false;
    }
}

void Game::handleCSelectEvents(SDL_Event e) {
    if (!cSelectDone) {
        for (unsigned int i = 0; i < cSelectUnitButtons.size(); i++){
            if (checkMouseEvent(cSelectUnitButtons[i], e) == 1) {
                addUnitToRoster(cSelectUnitButtons[i]->getText());
            }
        }
    } else {
        if (checkMouseEvent(cSelectStartButton, e) == 1) {
            gameState = play;
            initializeMatch();
        }
    }
}

void Game::handlePlayEvents(SDL_Event e) {

}

void Game::addUnitToRoster(std::string unit) {
    if (playerTurn == PLAYER1) {
        if (player1->hasUnit(unit)) {
            std::cout << "Unit already in roster!" << std::endl;
            return;
        }
        player1->addUnit(createUnit(unit), 1);

        Text* unitText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
        unitText->setText(unit, colorMap["light blue"]);
        player1SelectText.push_back(unitText);

        playerTurn = PLAYER2;

    } else {
        if (player2->hasUnit(unit)) {
            std::cout << "Unit already in roster!" << std::endl;
            return;
        }
        player2->addUnit(createUnit(unit), 2);
        
        Text* unitText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
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

    timelineHeader = new Text(renderer, "Terminal.ttf", 32, scaleX, scaleY);
    timelineHeader->setText("Timeline", colorMap["white"]);
    
    for (unsigned int i = 0; i < gameUnits.size(); i++) {
        Text* unitText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
        if (gameUnits[i]->getPlayerNum() == 1) {
            unitText->setText("P1: " + gameUnits[i]->getName(), colorMap["light blue"]);
        } else {
            unitText->setText("P2: " + gameUnits[i]->getName(), colorMap["light red"]);
        }
        
        timeline.push_back(unitText);
    }

    for (unsigned int i = 0; i < player1->getUnits().size(); i++) {
        Button* newButton = new Button(renderer, "Terminal.ttf", 12, player1->getUnits()[i]->getName(), colorMap["white"], colorMap["dark blue"], 120, 40, scaleX, scaleY);
        newButton->setOutline(true, colorMap["black"]);
        playUnitButtons.push_back(newButton);

        Text* newHpText = new Text(renderer, "Terminal.ttf", 12, scaleX, scaleY);
        int currentHp = player1->getUnits()[i]->getCurrHp();
        int maxHp = player1->getUnits()[i]->getMaxHp();
        std::string hpString = "HP: " + std::to_string(currentHp) + "/" + std::to_string(maxHp);
        newHpText->setText(hpString, colorMap["white"]);
        playUnitTexts.push_back(newHpText);
    }
    for (unsigned int i = 0; i < player2->getUnits().size(); i++) {
        Button* newButton = new Button(renderer, "Terminal.ttf", 12, player2->getUnits()[i]->getName(), colorMap["white"], colorMap["dark red"], 120, 40, scaleX, scaleY);
        newButton->setOutline(true, colorMap["black"]);
        playUnitButtons.push_back(newButton);

        Text* newHpText = new Text(renderer, "Terminal.ttf", 12, scaleX, scaleY);
        int currentHp = player2->getUnits()[i]->getCurrHp();
        int maxHp = player2->getUnits()[i]->getMaxHp();
        std::string hpString = "HP: " + std::to_string(currentHp) + "/" + std::to_string(maxHp);
        newHpText->setText(hpString, colorMap["white"]);
        playUnitTexts.push_back(newHpText);
    }
    
    playerTurnText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
    announcerText->setText("", colorMap["white"]);
    manaText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);

    populateUnitButtonMap();
    endTurn = false;
}

void Game::populateUnitButtonMap() {
    Button* attackButton = new Button(renderer, "Terminal.ttf", 16, "Attack", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
    attackButton->setOutline(true, colorMap["black"]);

    Button* skipButton = new Button(renderer, "Terminal.ttf", 16, "Skip Turn", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
    skipButton->setOutline(true, colorMap["black"]);

    for (unsigned int i = 0; i < gameUnits.size(); i++) {
        if (unitButtonMap[gameUnits[i]->getName()].size() > 0) {
            continue;
        }
        if (gameUnits[i]->getName() == "The Medic") {
            Button* healButton = new Button(renderer, "Terminal.ttf", 16, "Heal", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
            healButton->setOutline(true, colorMap["black"]);
            Button* medicAttackButton = new Button(renderer, "Terminal.ttf", 16, "Attack", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
            medicAttackButton->setOutline(true, colorMap["black"]);
            Button* specialButton = new Button(renderer, "Terminal.ttf", 16, "Health Pack", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
            specialButton->setOutline(true, colorMap["black"]);
            Button* medicSkipButton = new Button(renderer, "Terminal.ttf", 16, "Skip Turn", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
            medicSkipButton->setOutline(true, colorMap["black"]);

            unitButtonMap["The Medic"].push_back(healButton);
            unitButtonMap["The Medic"].push_back(medicAttackButton);
            unitButtonMap["The Medic"].push_back(specialButton);
            unitButtonMap["The Medic"].push_back(medicSkipButton);
        } else {
            Button* specialButton = new Button(renderer, "Terminal.ttf", 16, gameUnits[i]->getSpecialName(), colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
            specialButton->setOutline(true, colorMap["black"]);

            unitButtonMap[gameUnits[i]->getName()].push_back(attackButton);
            unitButtonMap[gameUnits[i]->getName()].push_back(specialButton);
            unitButtonMap[gameUnits[i]->getName()].push_back(skipButton);
        }
    }  
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
            playerTurnText->setText("Player 1's Turn: " + currentUnit->getName(), colorMap["light blue"]);
            manaText->setText("Mana: " + std::to_string(player1->getMana()), colorMap["light blue"]);
        } else {
            playerTurnText->setText("Player 2's Turn: " + currentUnit->getName(), colorMap["light red"]);
            manaText->setText("Mana: " + std::to_string(player2->getMana()), colorMap["light red"]);
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
        titleStartButton->render(400, 220);
        quitButton->render(400, 340);
    } else if (gameState == cSelect) {
        announcerText->render(25, 450);
        tempText->render(50, 300);
        cSelectStartButton->render(475, 430);
        for (unsigned int i = 0; i < cSelectUnitButtons.size(); i++) {
            if (i < 4) {
                cSelectUnitButtons[i]->render((i*175)+25, 50);
            } else {
                cSelectUnitButtons[i]->render(((i-4)*175)+25, 150);
            }
            
        }
        for (unsigned int i = 0; i < player1SelectText.size(); i++) {
            player1SelectText[i]->render(725, ((i)*35)+50);
        }
        for (unsigned int i = 0; i < player2SelectText.size(); i++) {
            player2SelectText[i]->render(725, ((i)*35)+270);
        }
    } else if (gameState == play) {
        playerTurnText->render(25, 340);
        announcerText->render(25, 380);
        manaText->render(500, 340);
        timelineHeader->render(725, 50);
        for (unsigned int i = 0; i < timeline.size(); i++) {
            timeline[i]->render(675, ((i+2)*35)+25);
        }
        for (unsigned int i = 0; i < 8; i++) {
            if (i < 4) {
                playUnitButtons[i]->render((i*150)+25, 50);
                playUnitTexts[i]->render((i*150)+27, 100);
            } else {
                playUnitButtons[i]->render(((i-4)*150)+25, 175);
                playUnitTexts[i]->render(((i-4)*150)+27, 225);
            }
            
        }
        if (currentUnit != nullptr) {
            for (unsigned int i = 0; i < unitButtonMap[currentUnit->getName()].size(); i++) {
                unitButtonMap[currentUnit->getName()][i]->render((i*175)+25, 425);
            }
        }
        
    }

    SDL_RenderPresent(renderer);
}