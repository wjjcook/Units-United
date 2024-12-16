#include "character_select_screen.hpp"

CharacterSelectScreen::CharacterSelectScreen(SDL_Renderer* renderer, float scaleX, float scaleY) : UI(renderer, scaleX, scaleY) {
    initializeElements();
}

CharacterSelectScreen::~CharacterSelectScreen() {
    delete announcerText;
    delete tempText;
    delete startButton;

    for (unsigned int i = 0; i < player1SelectText.size(); i++) {
        delete player1SelectText[i];
    }

    for (unsigned int i = 0; i < player2SelectText.size(); i++) {
        delete player2SelectText[i];
    }
    
    for (unsigned int i = 0; i < unitButtons.size(); i++) {
        delete unitButtons[i];
    }
}

void CharacterSelectScreen::initializeElements() {
    announcerText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
    announcerText->setText(" ", colorMap["white"]);

    tempText = new Text(renderer, "Terminal.ttf", 48, scaleX, scaleY);
    tempText->setText("NEW UNITS COMING SOON...", colorMap["white"]);

    startButton = new Button(renderer, "Terminal.ttf", 24, "Ready!", colorMap["white"], colorMap["green"], 150, 60, scaleX, scaleY);
    startButton->setOutline(true, colorMap["black"]);

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
    
    unitButtons.push_back(blademasterButton);
    unitButtons.push_back(cavemanButton);
    unitButtons.push_back(duelistButton);
    unitButtons.push_back(fighterButton);
    unitButtons.push_back(medicButton);
    unitButtons.push_back(raidBossButton);
    unitButtons.push_back(tankButton);
    unitButtons.push_back(villageIdiotButton);
}

void CharacterSelectScreen::setLocalPlayer(int playerNum) {
    localPlayer = playerNum;
}

void CharacterSelectScreen::setText(TextType type, std::string newText) {
    switch (type) {
    case TextType::Announcer:
        announcerText->setText(newText, colorMap["white"]);
        break;
    default:
        std::cout << "Unsupported text type" << std::endl;
        break;
    }
}

void CharacterSelectScreen::addUnitToPlayerSelectText(std::string unit, int playerNum) {
    Text* unitText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
    if (playerNum == 1) {
        unitText->setText(unit, colorMap["light blue"]);
        player1SelectText.push_back(unitText);
    } else {
        unitText->setText(unit, colorMap["light red"]);
        player2SelectText.push_back(unitText);
    }
}

Button* CharacterSelectScreen::getButton(ButtonType type) {
    switch (type) {
    case ButtonType::Start:
        return startButton;
    default:
        std::cout << "Unsupported button type" << std::endl;
        return nullptr;
    }
}

std::vector<Button*> CharacterSelectScreen::getUnitButtons() {
    return unitButtons;
}

void CharacterSelectScreen::render() {
    announcerText->render(25, 450);
    tempText->render(50, 300);
    startButton->render(725, 430);
    for (unsigned int i = 0; i < unitButtons.size(); i++) {
        if (i < 4) {
            unitButtons[i]->render((i*175)+25, 50);
        } else {
            unitButtons[i]->render(((i-4)*175)+25, 150);
        }
        
    }
    if (localPlayer == 1) {
        for (unsigned int i = 0; i < player1SelectText.size(); i++) {
            player1SelectText[i]->render(725, ((i)*35)+50);
        }
    } else {
        for (unsigned int i = 0; i < player2SelectText.size(); i++) {
            player2SelectText[i]->render(725, ((i)*35)+50);
        }
    }
}
