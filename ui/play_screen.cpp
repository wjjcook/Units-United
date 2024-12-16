#include "play_screen.hpp"

PlayScreen::PlayScreen(SDL_Renderer* renderer, float scaleX, float scaleY, std::vector<Unit*> player1Units, std::vector<Unit*> player2Units, std::vector<Unit*> unitsById) : UI(renderer, scaleX, scaleY) {
    this->player1Units = player1Units;
    this->player2Units = player2Units;
    this->unitsById = unitsById;
    initializeElements();
}

PlayScreen::~PlayScreen() {
    
    delete announcerText;
    delete timelineHeader;
    delete playerTurnText;
    delete manaText;
    delete rematchButton;
    delete quitButton;

    for (unsigned int i = 0; i < timeline.size(); i++) {
        delete timeline[i];
    }

    for (unsigned int i = 0; i < unitHpTexts.size(); i++) {
        delete unitHpTexts[i];
    }
    
    for (unsigned int i = 0; i < unitSelectButtons.size(); i++) {
        delete unitSelectButtons[i];
    }
}

void PlayScreen::initializeElements() {
    announcerText = new Text(renderer, "Terminal.ttf", 16, scaleX, scaleY);
    announcerText->setText(" ", colorMap["white"]);

    timelineHeader = new Text(renderer, "Terminal.ttf", 32, scaleX, scaleY);
    timelineHeader->setText("Timeline", colorMap["white"]);

    for (unsigned int i = 0; i < player1Units.size(); i++) {
        Button* newButton = new Button(renderer, "Terminal.ttf", 12, player1Units[i]->getName(), colorMap["white"], colorMap["dark blue"], 120, 40, scaleX, scaleY);
        newButton->setOutline(true, colorMap["black"]);
        unitSelectButtons.push_back(newButton);

        Text* newHpText = new Text(renderer, "Terminal.ttf", 12, scaleX, scaleY);
        int currentHp = player1Units[i]->getCurrHp();
        int maxHp = player1Units[i]->getMaxHp();
        std::string hpString = "HP: " + std::to_string(currentHp) + "/" + std::to_string(maxHp);
        newHpText->setText(hpString, colorMap["white"]);
        unitHpTexts.push_back(newHpText);
    }
    for (unsigned int i = 0; i < player2Units.size(); i++) {
        Button* newButton = new Button(renderer, "Terminal.ttf", 12, player2Units[i]->getName(), colorMap["white"], colorMap["dark red"], 120, 40, scaleX, scaleY);
        newButton->setOutline(true, colorMap["black"]);
        unitSelectButtons.push_back(newButton);

        Text* newHpText = new Text(renderer, "Terminal.ttf", 12, scaleX, scaleY);
        int currentHp =player2Units[i]->getCurrHp();
        int maxHp = player2Units[i]->getMaxHp();
        std::string hpString = "HP: " + std::to_string(currentHp) + "/" + std::to_string(maxHp);
        newHpText->setText(hpString, colorMap["white"]);
        unitHpTexts.push_back(newHpText);
    }

    playerTurnText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
    manaText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);

    rematchButton = new Button(renderer, "Terminal.ttf", 24, "Rematch", colorMap["white"], colorMap["green"], 150, 60, scaleX, scaleY);
    rematchButton->setOutline(true, colorMap["black"]);

    quitButton = new Button(renderer, "Terminal.ttf", 24, "Quit", colorMap["white"], colorMap["dark red"], 150, 60, scaleX, scaleY);
    quitButton->setOutline(true, colorMap["black"]);
    
    populateUnitButtonMap();
}

void PlayScreen::populateUnitButtonMap() {
    Button* attackButton = new Button(renderer, "Terminal.ttf", 16, "Attack", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
    attackButton->setOutline(true, colorMap["black"]);

    Button* skipButton = new Button(renderer, "Terminal.ttf", 16, "Skip Turn", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
    skipButton->setOutline(true, colorMap["black"]);

    Button* cancelButton = new Button(renderer, "Terminal.ttf", 16, "Cancel", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
    cancelButton->setOutline(true, colorMap["black"]);

    for (unsigned int i = 0; i < unitsById.size(); i++) {
        if (unitButtonMap[unitsById[i]->getName()].size() > 0) {
            continue;
        }
        if (unitsById[i]->getName() == "The Medic") {
            Button* healButton = new Button(renderer, "Terminal.ttf", 16, "Heal", colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
            healButton->setOutline(true, colorMap["black"]);
            unitButtonMap["The Medic"].push_back(healButton);
        
        } 
        Button* specialButton = new Button(renderer, "Terminal.ttf", 16, unitsById[i]->getSpecialName(), colorMap["white"], colorMap["grey"], 150, 60, scaleX, scaleY);
        specialButton->setOutline(true, colorMap["black"]);

        unitButtonMap[unitsById[i]->getName()].push_back(attackButton);
        unitButtonMap[unitsById[i]->getName()].push_back(specialButton);
        unitButtonMap[unitsById[i]->getName()].push_back(skipButton);
        unitButtonMap[unitsById[i]->getName()].push_back(cancelButton);
    }  
}

void PlayScreen::setText(TextType type, std::string newText) {
    switch (type) {
    case TextType::Announcer:
        announcerText->setText(newText, colorMap["white"]);
        break;
    case TextType::PlayerTurn:
        if (newText.find("Player 1") != std::string::npos){
            playerTurnText->setText(newText, colorMap["light blue"]);
        } else {
            playerTurnText->setText(newText, colorMap["light red"]);
        }
        break;
    case TextType::Mana:
        if (newText.back() == '1') {
            newText.pop_back();
            manaText->setText(newText, colorMap["light blue"]);
        } else {
            newText.pop_back();
            manaText->setText(newText, colorMap["light red"]);
        }
        break;
    default:
        std::cout << "Unsupported text type" << std::endl;
        break;
    }
}

Button* PlayScreen::getButton(ButtonType type) {
    switch (type) {
    case ButtonType::Rematch:
        return rematchButton;
    case ButtonType::Quit:
        return quitButton;
    default:
        std::cout << "Unsupported button type" << std::endl;
        return nullptr;
    }
}

std::vector<Button*> PlayScreen::getUnitButtons(std::string unit) {
    return unitButtonMap[unit];
}

std::vector<Button*> PlayScreen::getUnitSelectButtons() {
    return unitSelectButtons;
}

void PlayScreen::updateTimelineText(std::deque<Text*> newTimeline) {
    for (Text* text : timeline) {
        delete text;
    }
    timeline.clear();
    timeline = newTimeline;
}

void PlayScreen::updateUnitUI(Unit* unit) {
    std::string hpString = "HP: " + std::to_string(unit->getCurrHp()) + "/" + std::to_string(unit->getMaxHp());
    unitHpTexts[unit->getId()]->setText(hpString, colorMap["white"]);
}

void PlayScreen::updateUIAfterAttack(Unit* attacker, Unit* victim, int dmg, std::string customAnnouncement) {
    updateUnitUI(attacker);
    updateUnitUI(victim);

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

void PlayScreen::render() {
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
                unitSelectButtons[i]->render((i*150)+25, 50);
                unitHpTexts[i]->render((i*150)+27, 100);
            } else {
                unitSelectButtons[i]->render(((i-4)*150)+25, 175);
                unitHpTexts[i]->render(((i-4)*150)+27, 225);
            }
        } 
    }
}

void PlayScreen::renderEndScreen() {
    announcerText->render(275, 380);
    timelineHeader->render(725, 50);
    for (unsigned int i = 0; i < timeline.size(); i++) {
        timeline[i]->render(675, ((i+2)*35)+25);
    }
    for (unsigned int i = 0; i < 8; i++) {
        if (unitsById[i]->isAlive()) {
            if (i < 4) {
                unitSelectButtons[i]->render((i*150)+25, 50);
                unitHpTexts[i]->render((i*150)+27, 100);
            } else {
                unitSelectButtons[i]->render(((i-4)*150)+25, 175);
                unitHpTexts[i]->render(((i-4)*150)+27, 225);
            }
        } 
    }
    rematchButton->render(200, 425);
    quitButton->render(375, 425);
}