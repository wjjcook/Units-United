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
    inputText = "";

    player1 = nullptr;
    player2 = nullptr;
    playerTurn = PLAYER1;

    currentUnit = nullptr;

    fighterFirstTarget = "";
    fighterFirstAttack = 0;
    
}

Game::~Game() {

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

    titleScreen = new TitleScreen(renderer, scaleX, scaleY);
    cSelectScreen = new CharacterSelectScreen(renderer, scaleX, scaleY);

    initializeColors();
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
                        inputText += e.text.text;
                    } else if (e.type == SDL_KEYDOWN) {
                        if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0) {
                            inputText.pop_back();
                        } else if (e.key.keysym.sym == SDLK_RETURN) {
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
                        CharacterSelectScreen* cSelect = static_cast<CharacterSelectScreen*>(cSelectScreen);
                        cSelect->setLocalPlayer(2);
                    }
                    titleScreen->setText(TextType::Announcer, "Enter Remote IP: " + inputText);
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
    if (!searchForClient && checkMouseEvent(titleScreen->getButton(ButtonType::Start), e) == 1) {
        receiveIpInput = false;
        titleScreen->setText(TextType::Announcer, "Finding Match...");
        if (!initializeServer(12345)) {
            std::cerr << "Failed to initialize network." << std::endl;
            return;
        }
        titleScreen->getButton(ButtonType::Start)->setHovered(false);
    }
    if (checkMouseEvent(titleScreen->getButton(ButtonType::Join), e) == 1) {
        if (searchForClient) {
            closeServer();
            searchForClient = false;
        }
        titleScreen->setText(TextType::Announcer, "Enter Remote IP: ");
        SDL_StartTextInput();
        ipInput = "";
        ipInputDone = false;
        receiveIpInput = true;
    }
    if (checkMouseEvent(titleScreen->getButton(ButtonType::Quit), e) == 1) {
        if (searchForClient) {
            closeServer();
            searchForClient = false;
        }
        running = false;
    }
}

void Game::handleCSelectEvents(SDL_Event e) {
    CharacterSelectScreen* cSelect = static_cast<CharacterSelectScreen*>(cSelectScreen);
    if (!cSelectDone) {
        if (!(player1->isLocalPlayer() && player1->getUnits().size() >= 4) && !(player2->isLocalPlayer() && player2->getUnits().size() >= 4)) {
            for (unsigned int i = 0; i < cSelect->getUnitButtons().size(); i++){
                if (checkMouseEvent(cSelect->getUnitButtons()[i], e) == 1) {
                    addUnitToRoster(cSelect->getUnitButtons()[i]->getText());
                }
            }
        } else {
            if (checkMouseEvent(cSelect->getButton(ButtonType::Start), e) == 1) {
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
    CharacterSelectScreen* cSelect = static_cast<CharacterSelectScreen*>(cSelectScreen);
    if (player1->isLocalPlayer()) {
        if (player1->hasUnit(unit)) {
            std::cout << "Unit already in roster!" << std::endl;
            return;
        }
        player1->addUnit(createUnit(unit), 1);
        cSelect->addUnitToPlayerSelectText(unit, 1);
    } else {
        if (player2->hasUnit(unit)) {
            std::cout << "Unit already in roster!" << std::endl;
            return;
        }
        player2->addUnit(createUnit(unit), 2);
        cSelect->addUnitToPlayerSelectText(unit, 2);
    }
}

void Game::initializeMatch() {

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

    playScreen = new PlayScreen(renderer, scaleX, scaleY, player1->getUnits(), player2->getUnits(), unitsById);
    updateTimelineText();
    turnState = selectAction;
}

void Game::updateTimelineText() {
    std::deque<Text*> ttimeline;
    for (unsigned int i = 0; i < gameUnits.size(); i++) {
        Text* unitText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
        if (gameUnits[i]->getPlayerNum() == 1) {
            unitText->setText("P1: " + gameUnits[i]->getName(), colorMap["light blue"]);
        } else {
            unitText->setText("P2: " + gameUnits[i]->getName(), colorMap["light red"]);
        }
        
        ttimeline.push_back(unitText);
    }
    PlayScreen* pScreen = static_cast<PlayScreen*>(playScreen);
    pScreen->updateTimelineText(ttimeline);
}

void Game::handlePlayEvents(SDL_Event e) {
    if (!currentUnit) {
        return;
    }
    PlayScreen* pScreen = static_cast<PlayScreen*>(playScreen);
    std::vector<Button*> unitButtons = pScreen->getUnitButtons(currentUnit->getName());
    if (turnState == selectAction) {
        for (unsigned int i = 0; i < unitButtons.size(); i++) {
            if (checkMouseEvent(unitButtons[i], e) == 1) {
                std::string currentButtonText = unitButtons[i]->getText();
                if (currentButtonText == "Heal") {
                    // turnState = healAlly;
                    turnState = attackEnemy; // Temp for now
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
                    pScreen->setText(TextType::Announcer, currentAnnouncement);
                }
                unitButtons[i]->setHovered(false);
                break;
            }
        }
    } else if (turnState == attackEnemy) {
        unsigned int start = (playerTurn == PLAYER2) ? 0 : 4;
        unsigned int end = start + 4;

        if (checkMouseEvent(unitButtons.back(), e) == 1) {
            fighterFirstTarget = "";
            fighterFirstAttack = 0;
            turnState = selectAction;
        }
        std::vector<Button*> unitSelectButtons = pScreen->getUnitSelectButtons();
        for (unsigned int i = start; i < end; i++) {
            if (checkMouseEvent(unitSelectButtons[i], e) == 1) {
                if (currentUnit->getName() == "The Fighter" && ((playerTurn == PLAYER1 ? player2->aliveUnitCount() : player1->aliveUnitCount()) > 1)) {
                    if (fighterFirstTarget == ""){
                        fighterFirstTarget = unitSelectButtons[i]->getText();
                    } else {
                        initiateAttackOnClick(fighterFirstTarget);
                        initiateAttackOnClick(unitSelectButtons[i]->getText());
                        fighterFirstAttack = 0;
                        fighterFirstTarget = "";
                        Fighter* theFighter = static_cast<Fighter*>(currentUnit);
                        theFighter->setStandingGround(true);
                    }
                } else {
                    initiateAttackOnClick(unitSelectButtons[i]->getText());
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

                    if (currentUnit->getName() == "The Fighter" && ((playerTurn == PLAYER1 ? player2->aliveUnitCount() : player1->aliveUnitCount()) > 1)) {
                        if (fighterFirstAttack == 0) {
                            fighterFirstAttack = -beforeDamagePassives[i].getValue();
                        } else {
                            PlayScreen* pScreen = static_cast<PlayScreen*>(playScreen);
                            std::string customAnnouncement = "The Fighter attacked " + fighterFirstTarget + " for " + std::to_string(fighterFirstAttack) + " damage and The Duelist countered The Fighter for " + std::to_string(beforeDamagePassives[i].getValue()) + " damage!";
                            pScreen->updateUIAfterAttack(currentUnit, gameUnits[j], beforeDamagePassives[i].getValue(), customAnnouncement);
                        }
                    }

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
    PlayScreen* pScreen = static_cast<PlayScreen*>(playScreen);
    if (attacker->getName() == "The Fighter" && ((playerTurn == PLAYER1 ? player2->aliveUnitCount() : player1->aliveUnitCount()) > 1)) {
        if (fighterFirstAttack == 0) {
            fighterFirstAttack = newDmg;
            pScreen->updateUIAfterAttack(attacker, victim, newDmg);
        } else {
            std::string customAnnouncement;
            if (fighterFirstAttack < 0) {
                customAnnouncement = "The Duelist countered The Fighter for " + std::to_string(-fighterFirstAttack) + " damage and The Fighter attacked " + victim->getName() + " for " + std::to_string(newDmg) + " damage!";
            } else {
                customAnnouncement = "The Fighter attacked " + fighterFirstTarget + " for " + std::to_string(fighterFirstAttack) + " damage and " + victim->getName() + " for " + std::to_string(newDmg) + " damage!";
            }
            pScreen->updateUIAfterAttack(attacker, victim, newDmg, customAnnouncement);
            fighterFirstAttack = 0;
            fighterFirstTarget = "";
        }
    } else {
        pScreen->updateUIAfterAttack(attacker, victim, newDmg);
    }

    AttackMessage attackMsg(attacker->getId(), victim->getId(), rawDmg);
    sendMessage(attackMsg);

    turnState = endTurn;
    pScreen->getUnitSelectButtons()[victim->getId()]->setHovered(false);
}

void Game::sendPassiveEvents(std::vector<PassiveEventMessage> events) {
    for (PassiveEventMessage event : events) {
        sendMessage(event);
    }
}

void Game::updateUIAfterAttack(Unit* attacker, Unit* victim, int dmg, std::string customAnnouncement) {
    PlayScreen* pScreen = static_cast<PlayScreen*>(playScreen);
    pScreen->updateUIAfterAttack(attacker, victim, dmg, customAnnouncement);
}

void Game::handleEndEvents(SDL_Event e) {
    if (checkMouseEvent(playScreen->getButton(ButtonType::Rematch), e) == 1) {
        resetGame();
    }
    if (checkMouseEvent(playScreen->getButton(ButtonType::Quit), e) == 1) {
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
                CharacterSelectScreen* cSelect = static_cast<CharacterSelectScreen*>(cSelectScreen);
                cSelect->setLocalPlayer(1);
            }
        }
    } else if (gameState == cSelect) {
        CharacterSelectScreen* cSelect = static_cast<CharacterSelectScreen*>(cSelectScreen);
        if (!cSelectDone) {
            if ((player1->isLocalPlayer() && player1->getUnits().size() >= 4) || (player2->isLocalPlayer() && player2->getUnits().size() >= 4)) {
                cSelect->setText(TextType::Announcer, "Ready to start!");
                for (unsigned int i = 0; i < cSelect->getUnitButtons().size(); i++){
                    cSelect->getUnitButtons()[i]->setHovered(false);
                }
            } else {
                cSelect->setText(TextType::Announcer, "Select your units!");
            }
        } else {
            if (player1->isLocalPlayer()) {
                cSelect->setText(TextType::Announcer, "Waiting for Player 2 to finish character selection!");
            } else {
                cSelect->setText(TextType::Announcer, "Waiting for Player 1 to finish character selection!");
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
        PlayScreen* pScreen = static_cast<PlayScreen*>(playScreen);
        if (!player1->areAllUnitsAlive()) {
            winner = PLAYER2;
            pScreen->setText(TextType::Announcer, "Player 2 Wins!");
            gameState = end;
            return;
        }
        if (!player2->areAllUnitsAlive()) {
            winner = PLAYER1;
            pScreen->setText(TextType::Announcer, "Player 1 Wins!");
            gameState = end;
            return;
        }

        if (currentUnit == nullptr) {
            currentUnit = gameUnits.front();
        } else if (turnState == endTurn) {
            currentUnit = findNextUnit(currentUnit);
            if (currentUnit == gameUnits.front()) {
                roundOver();
            }
            currentUnit->onTurnPassives();
            pScreen->updateUnitUI(currentUnit);
            turnState = selectAction;
        }
        if (currentUnit->getPlayerNum() == 1) {
            playerTurn = PLAYER1;
        } else {
            playerTurn = PLAYER2;
        }

        if (playerTurn == PLAYER1) {
            pScreen->setText(TextType::PlayerTurn, "Player 1's Turn: " + currentUnit->getName());
            pScreen->setText(TextType::Mana, "Mana: " + std::to_string(player1->getMana()) + "1");
        } else {
            pScreen->setText(TextType::PlayerTurn, "Player 2's Turn: " + currentUnit->getName());
            pScreen->setText(TextType::Mana, "Mana: " + std::to_string(player2->getMana()) + "2");
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
                            pScreen->setText(TextType::Announcer, currentAnnouncement);
                        } else {
                            std::string currentAnnouncement = "Player 2 skipped " + currentUnit->getName() + "'s turn!";
                            pScreen->setText(TextType::Announcer, currentAnnouncement);
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
                            if (attacker->getName() == "The Fighter" && ((playerTurn == PLAYER1 ? player2->aliveUnitCount() : player1->aliveUnitCount()) > 1)) {
                                Fighter* theFighter = static_cast<Fighter*>(attacker);
                                theFighter->setStandingGround(true);
                                if (fighterFirstAttack == 0) {
                                    fighterFirstTarget = victim->getName();
                                    fighterFirstAttack = newDmg;
                                    pScreen->updateUIAfterAttack(attacker, victim, newDmg, customAnnouncement);
                                    break;
                                }
                                if (fighterFirstTarget == "countered") {
                                    customAnnouncement = "The Duelist countered The Fighter for " + std::to_string(fighterFirstAttack) + " damage and The Fighter attacked " + victim->getName() + " for " + std::to_string(newDmg) + " damage!";
                                } else {
                                    customAnnouncement = "The Fighter attacked " + fighterFirstTarget + " for " + std::to_string(fighterFirstAttack) + " damage and " + victim->getName() + " for " + std::to_string(newDmg) + " damage!";
                                }
                            }
                            if (customAnnouncement != "fighterCountered") {
                                pScreen->updateUIAfterAttack(attacker, victim, newDmg, customAnnouncement);
                                fighterFirstAttack = 0;
                                fighterFirstTarget = "";
                                turnState = endTurn;
                            }
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

void Game::roundOver() {
    for (unsigned int i = 0; i < gameUnits.size(); i++) {
        if (gameUnits[i]->getName() == "The Fighter") {
            Fighter* theFighter = static_cast<Fighter*>(gameUnits[i]);
            theFighter->setStandingGround(false);
        }
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
                    std::string customAnnouncement;
                    if (currentUnit->getName() == "The Fighter" && ((playerTurn == PLAYER1 ? player2->aliveUnitCount() : player1->aliveUnitCount()) > 1)) {
                        if (fighterFirstAttack == 0) {
                            fighterFirstTarget = "countered";
                            fighterFirstAttack = passiveMsg->getValue();
                            customAnnouncement = "fighterCountered";
                        } else {
                            customAnnouncement = "The Fighter attacked " + fighterFirstTarget + " for " + std::to_string(fighterFirstAttack) + " damage and The Duelist countered The Fighter for " + std::to_string(passiveMsg->getValue()) + " damage!";
                        }
                    } else {
                        customAnnouncement = "The Duelist countered " + currentUnit->getName() + " for " + std::to_string(firstAttack) + " damage!";
                    }                    
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
        titleScreen->render();
    } else if (gameState == cSelect) {
        cSelectScreen->render();
    } else if (gameState == play) {
        PlayScreen* pScreen = static_cast<PlayScreen*>(playScreen);
        pScreen->render();
        if (currentUnit != nullptr) {
            if ((playerTurn == PLAYER1 && player1->isLocalPlayer()) || (playerTurn == PLAYER2 && player2->isLocalPlayer())) {
                std::vector<Button*> unitButtonMap = pScreen->getUnitButtons(currentUnit->getName());
                for (unsigned int i = 0; i < unitButtonMap.size(); i++) {
                    if (i < unitButtonMap.size() - 1 || turnState == attackEnemy || turnState == specialEnemy || turnState == healAlly || turnState == specialAlly) {
                        unitButtonMap[i]->render((i*175)+25, 425);
                    }
                }
            }
        }
    } else if (gameState == end) {
        PlayScreen* pScreen = static_cast<PlayScreen*>(playScreen);
        pScreen->renderEndScreen();
    }

    SDL_RenderPresent(renderer);
}