#include "title_screen.hpp"

TitleScreen::TitleScreen(SDL_Renderer* renderer, float scaleX, float scaleY) : UI(renderer, scaleX, scaleY) {
    initializeElements();
}

TitleScreen::~TitleScreen() {
    delete titleText;
    delete ipText;
    delete announcerText;
    delete quitButton;
    delete joinButton;
    delete quitButton;
}

void TitleScreen::initializeElements() {
    titleText = new Text(renderer, "Terminal.ttf", 48, scaleX, scaleY);
    titleText->setText("Units United", colorMap["white"]);

    announcerText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
    announcerText->setText(" ", colorMap["white"]);

    ipText = new Text(renderer, "Terminal.ttf", 24, scaleX, scaleY);
    ipText->setText(" ", colorMap["white"]);

    startButton = new Button(renderer, "Terminal.ttf", 24, "Start Game", colorMap["white"], colorMap["green"], 150, 60, scaleX, scaleY);
    startButton->setOutline(true, colorMap["black"]);

    joinButton = new Button(renderer, "Terminal.ttf", 24, "Join Game", colorMap["white"], colorMap["green"], 150, 60, scaleX, scaleY);
    joinButton->setOutline(true, colorMap["black"]);

    quitButton = new Button(renderer, "Terminal.ttf", 24, "Quit", colorMap["white"], colorMap["dark red"], 150, 60, scaleX, scaleY);
    quitButton->setOutline(true, colorMap["black"]);
}

void TitleScreen::setText(TextType type, std::string newText) {
    switch (type) {
    case TextType::Announcer:
        announcerText->setText(newText, colorMap["white"]);
        break;
    case TextType::IP:
        ipText->setText(newText, colorMap["white"]);
        break;
    default:
        std::cout << "Unsupported text type" << std::endl;
        break;
    }
}

Button* TitleScreen::getButton(ButtonType type) {
    switch (type) {
    case ButtonType::Start:
        return startButton;
    case ButtonType::Join:
        return joinButton;
    case ButtonType::Quit:
        return quitButton; 
    default:
        std::cout << "Unsupported button type" << std::endl;
        return nullptr;
    }
}

void TitleScreen::render() {
    titleText->render(325, 100);
    announcerText->render(300, 200);
    startButton->render(300, 300);
    joinButton->render(500, 300);
    quitButton->render(400, 400);
}
