#ifndef PLAY_SCREEN_H
#define PLAY_SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <map>
#include <vector>
#include <deque>
#include <unordered_map>

#include "ui.hpp"
#include "text.hpp"
#include "button.hpp"
#include "../units/unit.hpp"



class PlayScreen : public UI {
    public:
        PlayScreen(SDL_Renderer* renderer, float scaleX, float scaleY, std::vector<Unit*> player1Units, std::vector<Unit*> player2Units, std::vector<Unit*> unitsById);
        ~PlayScreen() override;

        void initializeElements() override;
        void populateUnitButtonMap();

        void setText(TextType type, std::string newText) override;
        Button* getButton(ButtonType type) override;
        std::vector<Button*> getUnitButtons(std::string unit);
        std::vector<Button*> getUnitSelectButtons();
        void updateTimelineText(std::deque<Text*> newTimeline);
        void updateUnitUI(Unit* unit);
        void updateUIAfterAttack(Unit* attacker, Unit* victim, int dmg,std::string customAnnouncement = "");
        void render() override;
        void renderEndScreen();

    private:
        Text* playerTurnText;
        Text* announcerText;
        Text* manaText;
        Text* timelineHeader;
        std::deque<Text*> timeline;
        std::vector<Text*> unitHpTexts;

        Button* rematchButton;
        Button* quitButton;
        std::vector<Button*> unitSelectButtons;
        std::unordered_map<std::string, std::vector<Button*>> unitButtonMap;

        std::vector<Unit*> player1Units;
        std::vector<Unit*> player2Units;
        std::vector<Unit*> unitsById;     
};

#endif