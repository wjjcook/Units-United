#include "game.hpp"
#include <iostream>
#include <sstream>

Game::Game() {
    gameWindow = nullptr;
    renderer = nullptr;
    running = false;
    searchForClient = false;
    receiveIpInput = false;
    gameState = title;
    cSelectDone = false;
    remoteCSelectDone = false;

    titleText = nullptr;
    ipText = nullptr;
    playerTurnText = nullptr;
    announcerText = nullptr;
    timelineHeader = nullptr;
    manaText = nullptr;
    tempText = nullptr;
    inputText = "";

    titleStartButton = nullptr;
    titleJoinButton = nullptr;
    quitButton = nullptr;
    cSelectStartButton = nullptr;

    player1 = nullptr;
    player2 = nullptr;
    playerTurn = PLAYER1;

    currentUnit = nullptr;

    fighterFirstTarget = "";
    fighterFirstAttack = 0;
    
}

Game::~Game() {

    // Text objects
    delete titleText;
    delete ipText;
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

    for (unsigned int i = 0; i < playUnitHpTexts.size(); i++) {
        delete playUnitHpTexts[i];
    }

    // Button objects
    delete titleStartButton;
    delete titleJoinButton;
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
    SDL_StopTextInput();
    SDLNet_FreeSocketSet(socketSet);
    SDLNet_TCP_Close(client);
    SDLNet_Quit();
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

bool Game::initializeServer(Uint16 port) {
    if (SDLNet_Init() != 0) {
        std::cerr << "SDLNet_Init Error: " << SDLNet_GetError() << std::endl;
        return false;
    }

    if (SDLNet_ResolveHost(&ip, nullptr, port) != 0) {
        std::cerr << "SDLNet_ResolveHost Error: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return false;
    }

    server = SDLNet_TCP_Open(&ip);
    if (!server) {
        std::cerr << "SDLNet_TCP_Open Error: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return false;
    }

    searchForClient = true;
    
    return true;
}

void Game::closeServer() {
    if (client) {
        SDLNet_TCP_Close(client);
        client = nullptr;
    }

    if (server) {
        SDLNet_TCP_Close(server);
        server = nullptr;
    }

    SDLNet_Quit();

    std::cout << "Server closed." << std::endl;
}

bool Game::connectToServer(const char* serverIP, int port) {
    if (SDLNet_Init() != 0) {
        std::cerr << "SDLNet_Init Error: " << SDLNet_GetError() << std::endl;
        return false;
    }
    if (SDLNet_ResolveHost(&ip, serverIP, port) != 0) {
        std::cerr << "SDLNet_ResolveHost Error: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return false;
    }

    client = SDLNet_TCP_Open(&ip);
    if (!client) {
        std::cerr << "SDLNet_TCP_Open Error: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return false;
    }
    socketSet = SDLNet_AllocSocketSet(1);
    SDLNet_TCP_AddSocket(socketSet, client);
    return true;
}

void Game::sendMessage(const Message& msg) {
    char buffer[512];
    msg.serialize(buffer);
    SDLNet_TCP_Send(client, buffer, sizeof(buffer));
}

Message* Game::receiveMessage() {
    int activeSockets = SDLNet_CheckSockets(socketSet, 0);
    if (activeSockets > 0 && SDLNet_SocketReady(client)) {
        char buffer[512];
        memset(buffer, 0, sizeof(buffer));

        int received = SDLNet_TCP_Recv(client, buffer, sizeof(buffer));
        if (received > 0) {
            MessageType type;
            memcpy(&type, buffer, sizeof(type));
            Message* msg = Message::createMessage(type);
            if (msg) {
                msg->deserialize(buffer);
                return msg;
            }
        }
    }
    return nullptr;    
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

    announcerText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
    announcerText->setText(" ", colorMap["white"]);

    ipText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
    ipText->setText(" ", colorMap["white"]);

    titleStartButton = new Button(renderer, "Terminal.ttf", 24, "Start Game", colorMap["white"], colorMap["green"], 150, 60, scaleX, scaleY);
    titleStartButton->setOutline(true, colorMap["black"]);

    titleJoinButton = new Button(renderer, "Terminal.ttf", 24, "Join Game", colorMap["white"], colorMap["green"], 150, 60, scaleX, scaleY);
    titleJoinButton->setOutline(true, colorMap["black"]);

    quitButton = new Button(renderer, "Terminal.ttf", 24, "Quit", colorMap["white"], colorMap["dark red"], 150, 60, scaleX, scaleY);
    quitButton->setOutline(true, colorMap["black"]);
}

void Game::initializeCSelectElements(SDL_Renderer* renderer) {

    tempText = new Text(renderer, "Terminal.ttf", 48, scaleX, scaleY);
    tempText->setText("NEW UNITS COMING SOON...", colorMap["white"]);

    cSelectStartButton = new Button(renderer, "Terminal.ttf", 24, "Ready!", colorMap["white"], colorMap["green"], 150, 60, scaleX, scaleY);
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
        if (!running) {
            break;
        }
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
                if (receiveIpInput) {                   
                    if (e.type == SDL_TEXTINPUT) {
                        // Append new text to the current input text
                        inputText += e.text.text;
                    } else if (e.type == SDL_KEYDOWN) {
                        if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0) {
                            // Handle backspace
                            inputText.pop_back();
                        } else if (e.key.keysym.sym == SDLK_RETURN) {
                            // Handle Enter key (finish input)
                            std::cout << "Input: " << inputText << std::endl;
                            SDL_StopTextInput();
                            ipInputDone = true;
                            receiveIpInput = false;
                        }
                    } 
                    if (ipInputDone) {
                        if (!connectToServer(inputText.c_str(), 12345)) { 
                            std::cout << "Failed to connect to IP address" << std::endl;
                            return;
                        }
                        player1 = new Player();
                        player2 = new Player();
                        player2->setLocalPlayer(true);
                        gameState = cSelect;
                    }
                    announcerText->setText("Enter Remote IP: " + inputText, colorMap["white"]);
                }
                handleTitleEvents(e);
                
            } else if (gameState == cSelect) {
                handleCSelectEvents(e);
            } else if (gameState == play) {
                handlePlayEvents(e);
            } else if (gameState == end) {
                handleEndEvents(e);
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
    if (!searchForClient && checkMouseEvent(titleStartButton, e) == 1) {
        receiveIpInput = false;
        announcerText->setText("Finding Match...", colorMap["white"]);
        announcerText->render(400, 200);
        SDL_RenderPresent(renderer);
        if (!initializeServer(12345)) {
            std::cerr << "Failed to initialize network." << std::endl;
            return;
        }
        titleStartButton->setHovered(false);  
    }
    if (checkMouseEvent(titleJoinButton, e) == 1) {
        if (searchForClient) {
            closeServer();
            searchForClient = false;
        }
        announcerText->setText("Enter Remote IP: ", colorMap["white"]);
        SDL_StartTextInput();
        ipInput = "";
        ipInputDone = false;
        receiveIpInput = true;
    }
    if (checkMouseEvent(quitButton, e) == 1) {
        if (searchForClient) {
            closeServer();
            searchForClient = false;
        }
        running = false;
    }
}

void Game::handleCSelectEvents(SDL_Event e) {
    if (!cSelectDone) {
        if (!(player1->isLocalPlayer() && player1->getUnits().size() >= 4) && !(player2->isLocalPlayer() && player2->getUnits().size() >= 4)) {
            for (unsigned int i = 0; i < cSelectUnitButtons.size(); i++){
                if (checkMouseEvent(cSelectUnitButtons[i], e) == 1) {
                    addUnitToRoster(cSelectUnitButtons[i]->getText());
                }
            }
        } else {
            if (checkMouseEvent(cSelectStartButton, e) == 1) {
                cSelectDone = true;
                std::vector<std::string> unitNames;
                
                for (unsigned int i = 0; i < 4; i++) {
                    if (player1->isLocalPlayer()) {
                        unitNames.push_back(player1->getUnits()[i]->getName());
                    } else {
                        unitNames.push_back(player2->getUnits()[i]->getName());
                    }
                }
                CharacterSelectionMessage cSelectMsg(unitNames);
                sendMessage(cSelectMsg);
            }
        }
    }
}

void Game::addUnitToRoster(std::string unit) {
    if (player1->isLocalPlayer()) {
        if (player1->hasUnit(unit)) {
            std::cout << "Unit already in roster!" << std::endl;
            return;
        }
        player1->addUnit(createUnit(unit), 1);

        Text* unitText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
        unitText->setText(unit, colorMap["light blue"]);
        player1SelectText.push_back(unitText);
    } else {
        if (player2->hasUnit(unit)) {
            std::cout << "Unit already in roster!" << std::endl;
            return;
        }
        player2->addUnit(createUnit(unit), 2);
        
        Text* unitText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
        unitText->setText(unit, colorMap["light red"]);
        player2SelectText.push_back(unitText);
    }
}

void Game::initializeMatch() {

    delete announcerText;
    announcerText = new Text(renderer, "Terminal.ttf", 20, scaleX, scaleY);

    player1->sortUnitsBySpeed();
    player2->sortUnitsBySpeed();

    if (player1->isLocalPlayer()) {
        int i = 0;
        int j = 0;
        std::vector<std::pair<std::string, int>> unitOrder;
        while (i < 4 && j < 4) {
            if (player1->getUnits()[i]->getSpeed() > player2->getUnits()[j]->getSpeed()) {
                gameUnits.push_back(player1->getUnits()[i]);
                unitOrder.push_back({player1->getUnits()[i]->getName(), 1});
                i++;
            } else if (player1->getUnits()[i]->getSpeed() < player2->getUnits()[j]->getSpeed()) {
                gameUnits.push_back(player2->getUnits()[j]);
                unitOrder.push_back({player2->getUnits()[j]->getName(), 2});
                j++;
            } else {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dist(0, 1);
                int randomChoice = dist(gen);
                if (randomChoice == 0) {
                    gameUnits.push_back(player1->getUnits()[i]);
                    unitOrder.push_back({player1->getUnits()[i]->getName(), 1});
                    i++;
                } else {
                    gameUnits.push_back(player2->getUnits()[j]);
                    unitOrder.push_back({player2->getUnits()[j]->getName(), 2});
                    j++;
                }
            }
        }
        while (i < 4) {
            gameUnits.push_back(player1->getUnits()[i]);
            unitOrder.push_back({player1->getUnits()[i]->getName(), 1});
            i++;
        }
        while (j < 4) {
            gameUnits.push_back(player2->getUnits()[j]);
            unitOrder.push_back({player2->getUnits()[j]->getName(), 2});
            j++;
        }
        for (unsigned int i = 0; i < 8; i++) {
            gameUnits[i]->setId(i);
        }
        UnitOrderMessage unitOrderMsg(unitOrder);
        sendMessage(unitOrderMsg);
    } else {
        Message* receivedMsg = nullptr;
        while (!receivedMsg) {
            receivedMsg = receiveMessage();
        }
        if (receivedMsg->getType() == MessageType::UNIT_ORDER) {
            UnitOrderMessage* unitOrderMsg = static_cast<UnitOrderMessage*>(receivedMsg);
            int i = 0;
            int j = 0;
            for (unsigned int k = 0; k < 8; k++) {
                if (unitOrderMsg->getUnits()[k].second == 1) {
                    gameUnits.push_back(player1->getUnits()[i]);
                    i++;
                } else {
                    gameUnits.push_back(player2->getUnits()[j]);
                    j++;
                }
            }
            std::cout << "Units received" << std::endl;
        } else {
            std::cout << "Error: Wrong type of message received, trying again." << std::endl;
            delete receivedMsg;
            initializeMatch();
            return;
        }
        delete receivedMsg;
    }

    for (unsigned int i = 0; i < 4; i++) {
        player1->getUnits()[i]->setId(i);
        unitsById.push_back(player1->getUnits()[i]);
    }
    for (unsigned int i = 0; i < 4; i++) {
        player2->getUnits()[i]->setId(i+4);
        unitsById.push_back(player2->getUnits()[i]);
    }

    timelineHeader = new Text(renderer, "Terminal.ttf", 32, scaleX, scaleY);
    timelineHeader->setText("Timeline", colorMap["white"]);
    
    updateTimelineText();

    for (unsigned int i = 0; i < player1->getUnits().size(); i++) {
        Button* newButton = new Button(renderer, "Terminal.ttf", 12, player1->getUnits()[i]->getName(), colorMap["white"], colorMap["dark blue"], 120, 40, scaleX, scaleY);
        newButton->setOutline(true, colorMap["black"]);
        playUnitButtons.push_back(newButton);

        Text* newHpText = new Text(renderer, "Terminal.ttf", 12, scaleX, scaleY);
        int currentHp = player1->getUnits()[i]->getCurrHp();
        int maxHp = player1->getUnits()[i]->getMaxHp();
        std::string hpString = "HP: " + std::to_string(currentHp) + "/" + std::to_string(maxHp);
        newHpText->setText(hpString, colorMap["white"]);
        playUnitHpTexts.push_back(newHpText);
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
        playUnitHpTexts.push_back(newHpText);
    }
    
    playerTurnText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
    announcerText->setText(" ", colorMap["white"]);
    manaText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);

    rematchButton = new Button(renderer, "Terminal.ttf", 24, "Rematch", colorMap["white"], colorMap["green"], 150, 60, scaleX, scaleY);
    rematchButton->setOutline(true, colorMap["black"]);

    populateUnitButtonMap();
    turnState = selectAction;
}

void Game::updateTimelineText() {
    for (Text* text : timeline) {
        delete text;
    }
    timeline.clear();
    for (unsigned int i = 0; i < gameUnits.size(); i++) {
        Text* unitText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
        if (gameUnits[i]->getPlayerNum() == 1) {
            unitText->setText("P1: " + gameUnits[i]->getName(), colorMap["light blue"]);
        } else {
            unitText->setText("P2: " + gameUnits[i]->getName(), colorMap["light red"]);
        }
        
        timeline.push_back(unitText);
    }
}

void Game::populateUnitButtonMap() {
    Button* attackButton = new Button(renderer, "Terminal.ttf", 16, "Attack", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
    attackButton->setOutline(true, colorMap["black"]);

    Button* skipButton = new Button(renderer, "Terminal.ttf", 16, "Skip Turn", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
    skipButton->setOutline(true, colorMap["black"]);

    Button* cancelButton = new Button(renderer, "Terminal.ttf", 16, "Cancel", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
    cancelButton->setOutline(true, colorMap["black"]);

    for (unsigned int i = 0; i < gameUnits.size(); i++) {
        if (unitButtonMap[gameUnits[i]->getName()].size() > 0) {
            continue;
        }
        if (gameUnits[i]->getName() == "The Medic") {
            Button* healButton = new Button(renderer, "Terminal.ttf", 16, "Heal", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
            healButton->setOutline(true, colorMap["black"]);
            unitButtonMap["The Medic"].push_back(healButton);
        
        } 
        Button* specialButton = new Button(renderer, "Terminal.ttf", 16, gameUnits[i]->getSpecialName(), colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
        specialButton->setOutline(true, colorMap["black"]);

        unitButtonMap[gameUnits[i]->getName()].push_back(attackButton);
        unitButtonMap[gameUnits[i]->getName()].push_back(specialButton);
        unitButtonMap[gameUnits[i]->getName()].push_back(skipButton);
        unitButtonMap[gameUnits[i]->getName()].push_back(cancelButton);
    }  
}

void Game::handlePlayEvents(SDL_Event e) {
    if (!currentUnit) {
        return;
    }
    if (turnState == selectAction) {
        for (unsigned int i = 0; i < unitButtonMap[currentUnit->getName()].size(); i++) {
            if (checkMouseEvent(unitButtonMap[currentUnit->getName()][i], e) == 1) {
                std::string currentButtonText = unitButtonMap[currentUnit->getName()][i]->getText();
                if (currentButtonText == "Heal") {
                    turnState = healAlly;
                } else if (currentButtonText == "Attack") {
                    turnState = attackEnemy;
                } else if (currentButtonText == currentUnit->getSpecialName()) {
                    if (currentUnit->getSpecialTarget() == ally) {
                        turnState = specialAlly;
                    } else {
                        turnState = specialEnemy;
                    }
                } else if (currentButtonText == "Skip Turn") {
                    StringMessage skipMsg("skip");
                    sendMessage(skipMsg);
                    turnState = endTurn;
                    std::string currentAnnouncement = "You skipped " + currentUnit->getName() + "'s turn!";
                    announcerText->setText(currentAnnouncement, colorMap["white"]);
                }
                unitButtonMap[currentUnit->getName()][i]->setHovered(false);
                break;
            }
        }
    } else if (turnState == attackEnemy) {
        unsigned int start = (playerTurn == PLAYER2) ? 0 : 4;
        unsigned int end = start + 4;

        if (checkMouseEvent(unitButtonMap[currentUnit->getName()].back(), e) == 1) {
            fighterFirstTarget = "";
            fighterFirstAttack = 0;
            turnState = selectAction;
        }

        for (unsigned int i = start; i < end; i++) {
            if (checkMouseEvent(playUnitButtons[i], e) == 1) {
                if (currentUnit->getName() == "The Fighter") {
                    if (fighterFirstTarget == ""){
                        fighterFirstTarget = playUnitButtons[i]->getText();
                    } else {
                        initiateAttackOnClick(fighterFirstTarget);
                        initiateAttackOnClick(playUnitButtons[i]->getText());
                        fighterFirstAttack = 0;
                        fighterFirstTarget = "";
                    }
                } else {
                    initiateAttackOnClick(playUnitButtons[i]->getText());
                }
            }
        }
    } else if (turnState == healAlly) {
        turnState = endTurn; // Temporary
    }
    updateTimeline();
}

void Game::initiateAttackOnClick(std::string targetName) {
    for (unsigned int j = 0; j < 8; j++) {
        if (gameUnits[j]->getName() == targetName && gameUnits[j]->getPlayerNum() != playerTurn) {
            
            Unit* victim = gameUnits[j];
            if (!currentUnit->attackHits()) {
                unitAttack(currentUnit, victim, 0, 0);
                break;
            }

            std::vector<PassiveEventMessage> beforeDamagePassives = victim->beforeDamagePassives(*this, currentUnit);
            bool countered = false;
            for (unsigned int i = 0; i < beforeDamagePassives.size(); i++) {
                if (beforeDamagePassives[i].getPassiveType() == "counterAttack") {
                    countered = true;
                    break;
                }
            }
            sendPassiveEvents(beforeDamagePassives);

            if (!countered) {
                currentUnit->attack(*this, gameUnits[j]);
            }
            break;
        }
    }
}

void Game::unitAttack(Unit* attacker, Unit* victim, int rawDmg, int newDmg) {

    if (attacker->getName() == "The Fighter") {
        if (fighterFirstAttack == 0) {
            fighterFirstAttack = newDmg;
            updateUIAfterAttack(attacker, victim, newDmg);
        } else {
            // Need to account for duelist counters
            std::string customAnnouncement = "The Fighter attacked " + fighterFirstTarget + " for " + std::to_string(fighterFirstAttack) + " damage and " + victim->getName() + " for " + std::to_string(newDmg) + " damage!";
            updateUIAfterAttack(attacker, victim, newDmg, customAnnouncement);
            fighterFirstAttack = 0;
            fighterFirstTarget = "";
        }
    } else {
        updateUIAfterAttack(attacker, victim, newDmg);
    }

    AttackMessage attackMsg(attacker->getId(), victim->getId(), rawDmg);
    sendMessage(attackMsg);

    turnState = endTurn;
    playUnitButtons[victim->getId()]->setHovered(false);
}

void Game::sendPassiveEvents(std::vector<PassiveEventMessage> events) {
    for (PassiveEventMessage event : events) {
        sendMessage(event);
    }
}

void Game::updateUIAfterAttack(Unit* attacker, Unit* victim, int dmg, std::string customAnnouncement) {
    std::string attackerHpString = "HP: " + std::to_string(attacker->getCurrHp()) + "/" + std::to_string(attacker->getMaxHp());
    playUnitHpTexts[attacker->getId()]->setText(attackerHpString, colorMap["white"]);

    std::string victimHpString = "HP: " + std::to_string(victim->getCurrHp()) + "/" + std::to_string(victim->getMaxHp());
    playUnitHpTexts[victim->getId()]->setText(victimHpString, colorMap["white"]);

    std::string currentAnnouncement;
    if (customAnnouncement != "") {
        currentAnnouncement = customAnnouncement;
    } else if (dmg == 0) {
        currentAnnouncement = attacker->getName() + " missed " + victim->getName() + "!";
    } else {
        currentAnnouncement = attacker->getName() + " attacked " + victim->getName() + " for " + std::to_string(dmg) + " damage!";                            
    }
    announcerText->setText(currentAnnouncement, colorMap["white"]);
}

void Game::handleEndEvents(SDL_Event e) {
    if (checkMouseEvent(rematchButton, e) == 1) {
        resetGame();
    }
    if (checkMouseEvent(quitButton, e) == 1) {
        running = false;
    }
}

void Game::resetGame() {
    // gameState = cSelect;
    // cSelectDone = false;
    // remoteCSelectDone = false;
    return;
    
}

void Game::update() {
    if (gameState == title) {
        if (searchForClient) {
            client = SDLNet_TCP_Accept(server);
            if (client) {
                socketSet = SDLNet_AllocSocketSet(1);
                SDLNet_TCP_AddSocket(socketSet, client);
                std::cout << "Match Found!" << std::endl;
                searchForClient = false;
                player1 = new Player();
                player2 = new Player();
                player1->setLocalPlayer(true);
                gameState = cSelect;
            }
        }
    } else if (gameState == cSelect) {
        if (!cSelectDone) {
            if ((player1->isLocalPlayer() && player1->getUnits().size() >= 4) || (player2->isLocalPlayer() && player2->getUnits().size() >= 4)) {
                announcerText->setText("Ready to start!", colorMap["white"]);
                for (unsigned int i = 0; i < cSelectUnitButtons.size(); i++){
                    cSelectUnitButtons[i]->setHovered(false);
                }
            } else {
                announcerText->setText("Select your units!", colorMap["white"]);
            }
        } else {
            if (player1->isLocalPlayer()) {
                announcerText->setText("Waiting for Player 2 to finish character selection!", colorMap["white"]);
            } else {
                announcerText->setText("Waiting for Player 1 to finish character selection!", colorMap["white"]);
            }
        }
        Message* receivedMsg = receiveMessage();
        if (receivedMsg) {
            std::cout << "Message Received!" << std::endl;
            if (receivedMsg->getType() == MessageType::CHARACTER_SELECTION) {
                CharacterSelectionMessage* cSelectMsg = static_cast<CharacterSelectionMessage*>(receivedMsg);
                for (unsigned int i = 0; i < 4; i++) {
                    if (player1->isLocalPlayer()) {
                        player2->addUnit(createUnit(cSelectMsg->getUnits()[i]), 2);
                    } else {
                        player1->addUnit(createUnit(cSelectMsg->getUnits()[i]), 1);
                    }
                }
                remoteCSelectDone = true;
            }
            delete receivedMsg;
        }
        if (cSelectDone && remoteCSelectDone) {
            gameState = play;
            initializeMatch();
        }
        
    } else if (gameState == play) {

        if (!player1->areAllUnitsAlive()) {
            winner = PLAYER2;
            announcerText->setText("Player 2 Wins!", colorMap["white"]);
            gameState = end;
            return;
        }
        if (!player2->areAllUnitsAlive()) {
            winner = PLAYER1;
            announcerText->setText("Player 1 Wins!", colorMap["white"]);
            gameState = end;
            return;
        }

        if (currentUnit == nullptr) {
            currentUnit = gameUnits.front();
        } else if (turnState == endTurn) {
            currentUnit = findNextUnit(currentUnit);
            turnState = selectAction;
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

        if ((playerTurn == PLAYER1 && player2->isLocalPlayer()) || (playerTurn == PLAYER2 && player1->isLocalPlayer())) {
            
            Message* receivedMsg = receiveMessage();
            if (receivedMsg) {
                if (receivedMsg->getType() == MessageType::STRING) {
                    StringMessage* strMsg = static_cast<StringMessage*>(receivedMsg);
                    if (strMsg->getString() == "skip") {
                        turnState = endTurn;
                        if (playerTurn == PLAYER1) {
                            std::string currentAnnouncement = "Player 1 skipped " + currentUnit->getName() + "'s turn!";
                            announcerText->setText(currentAnnouncement, colorMap["white"]);
                        } else {
                            std::string currentAnnouncement = "Player 2 skipped " + currentUnit->getName() + "'s turn!";
                            announcerText->setText(currentAnnouncement, colorMap["white"]);
                        }
                        
                    }
                } else if (receivedMsg->getType() == MessageType::ATTACK) {
                    AttackMessage* attackMsg = static_cast<AttackMessage*>(receivedMsg);
                    Unit* attacker = nullptr;
                    Unit* victim = nullptr;
                    for (unsigned int i = 0; i < gameUnits.size(); i++) {
                        if (attackMsg->getAttackerId() == gameUnits[i]->getId()) {
                            attacker = gameUnits[i];
                        } else if (attackMsg->getTargetId() == gameUnits[i]->getId()) {
                            victim = gameUnits[i];
                        }
                        if (attacker && victim) {
                            int newDmg = 0;
                            std::string customAnnouncement = "";

                            if (attackMsg->getDamage() > 0) {
                                newDmg = victim->damageUnit(attackMsg->getDamage(), true, attacker);
                                customAnnouncement = receiveAndHandlePassiveMessages(newDmg);
                            }
                            if (attacker->getName() == "The Fighter") {
                                if (fighterFirstAttack == 0) {
                                    fighterFirstTarget = victim->getName();
                                    fighterFirstAttack = newDmg;
                                    updateUIAfterAttack(attacker, victim, newDmg, customAnnouncement);
                                    break;
                                }
                                // Need to account for duelist counters
                                customAnnouncement = "The Fighter attacked " + fighterFirstTarget + " for " + std::to_string(fighterFirstAttack) + " damage and " + victim->getName() + " for " + std::to_string(newDmg) + " damage!";
                            }
                            
                            updateUIAfterAttack(attacker, victim, newDmg, customAnnouncement);
                            fighterFirstAttack = 0;
                            fighterFirstTarget = "";
                            turnState = endTurn;
                            break;
                        }
                    }
                }
                delete receivedMsg;
            }
        }
        updateTimeline();
    }
}

std::string Game::receiveAndHandlePassiveMessages(int firstAttack) {
    bool messagesAvailable = true;
    bool passiveAnnouncement = false;

    Unit* victim = nullptr;
    std::vector<int> multiAttacks = {firstAttack};
    
    while (messagesAvailable) {
        Message* receivedPassiveMsg = receiveMessage();

        if (receivedPassiveMsg) {
            if (receivedPassiveMsg->getType() == MessageType::PASSIVE_EVENT) {
                PassiveEventMessage* passiveMsg = static_cast<PassiveEventMessage*>(receivedPassiveMsg);
                
                if (passiveMsg->getPassiveType() == "END") {
                    messagesAvailable = false;
                } else if (passiveMsg->getPassiveType() == "heal") {
                    currentUnit->setCurrHp(currentUnit->getCurrHp() + passiveMsg->getValue());
                } else if (passiveMsg->getPassiveType() == "dmgIncrease") {
                    currentUnit->increaseDmg(passiveMsg->getValue());
                } else if (passiveMsg->getPassiveType() == "additionalAttack") {
                    if (!victim) {
                        for (unsigned int i = 0; i < 8; i++) {
                            if (gameUnits[i]->getName() == passiveMsg->getUnitName() && gameUnits[i]->getPlayerNum() != currentUnit->getPlayerNum()) {
                                victim = gameUnits[i];
                                break;
                            }
                        }
                    } 
                    int newDmg = victim->damageUnit(passiveMsg->getValue(), true, currentUnit);
                    multiAttacks.push_back(newDmg);
                    passiveAnnouncement = true;

                } else if (passiveMsg->getPassiveType() == "counterAttack") {
                    std::string customAnnouncement = "The Duelist countered " + currentUnit->getName() + " for " + std::to_string(firstAttack) + " damage!";
                    delete receivedPassiveMsg;
                    return customAnnouncement;
                }
            }
            delete receivedPassiveMsg;
        }   
    }

    if (passiveAnnouncement) {
        return generateCustomAnnouncement(victim, multiAttacks);
    }
    return "";
}

std::string Game::generateCustomAnnouncement(Unit* victim, std::vector<int> multiAttacks) {
    if (currentUnit->getName() == "The Blademaster" && victim) {
        std::ostringstream oss;

        oss << "The Blademaster attacked " << victim->getName() << " 3 times for ";

        for (size_t i = 0; i < multiAttacks.size(); ++i) {
            oss << multiAttacks[i];
            if (i == multiAttacks.size() - 2) {
                oss << ", and ";
            } else if (i < multiAttacks.size() - 2) {
                oss << ", "; 
            }
        }

        oss << " damage!";
        return oss.str();
    }
    return "";
}

void Game::updateTimeline() {
    bool timelineChanged = false;
    for (auto it = gameUnits.begin(); it != gameUnits.end(); ) {
        if (!(*it)->isAlive()) {
            it = gameUnits.erase(it);
            timelineChanged = true;
        } else {
            ++it;
        }
    }
    if (timelineChanged) {
        updateTimelineText();
    }
}

Unit* Game::findNextUnit(Unit* currentUnit) {

    if (gameUnits.size() == 0) {
        return nullptr;
    }

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
        announcerText->render(300, 200);
        titleStartButton->render(300, 300);
        titleJoinButton->render(500, 300);
        quitButton->render(400, 400);
    } else if (gameState == cSelect) {
        announcerText->render(25, 450);
        tempText->render(50, 300);
        cSelectStartButton->render(725, 430);
        for (unsigned int i = 0; i < cSelectUnitButtons.size(); i++) {
            if (i < 4) {
                cSelectUnitButtons[i]->render((i*175)+25, 50);
            } else {
                cSelectUnitButtons[i]->render(((i-4)*175)+25, 150);
            }
            
        }
        if (player1->isLocalPlayer()) {
            for (unsigned int i = 0; i < player1SelectText.size(); i++) {
                player1SelectText[i]->render(725, ((i)*35)+50);
            }
        } else {
            for (unsigned int i = 0; i < player2SelectText.size(); i++) {
                player2SelectText[i]->render(725, ((i)*35)+50);
            }
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
            if (unitsById[i]->isAlive()) {
                if (i < 4) {
                    playUnitButtons[i]->render((i*150)+25, 50);
                    playUnitHpTexts[i]->render((i*150)+27, 100);
                } else {
                    playUnitButtons[i]->render(((i-4)*150)+25, 175);
                    playUnitHpTexts[i]->render(((i-4)*150)+27, 225);
                }
            } 
        }
        if (currentUnit != nullptr) {
            if ((playerTurn == PLAYER1 && player1->isLocalPlayer()) || (playerTurn == PLAYER2 && player2->isLocalPlayer())) {
                for (unsigned int i = 0; i < unitButtonMap[currentUnit->getName()].size(); i++) {
                    if (i < unitButtonMap[currentUnit->getName()].size() - 1 || turnState == attackEnemy || turnState == specialEnemy || turnState == healAlly || turnState == specialAlly) {
                        unitButtonMap[currentUnit->getName()][i]->render((i*175)+25, 425);
                    }
                }
            }
        }
        
    } else if (gameState == end) {
        announcerText->render(275, 380);
        timelineHeader->render(725, 50);
        for (unsigned int i = 0; i < timeline.size(); i++) {
            timeline[i]->render(675, ((i+2)*35)+25);
        }
        for (unsigned int i = 0; i < 8; i++) {
            if (unitsById[i]->isAlive()) {
                if (i < 4) {
                    playUnitButtons[i]->render((i*150)+25, 50);
                    playUnitHpTexts[i]->render((i*150)+27, 100);
                } else {
                    playUnitButtons[i]->render(((i-4)*150)+25, 175);
                    playUnitHpTexts[i]->render(((i-4)*150)+27, 225);
                }
            } 
        }
        rematchButton->render(200, 425);
        quitButton->render(375, 425);
    }

    SDL_RenderPresent(renderer);
}