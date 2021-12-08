#pragma once

#include "../../app/app.h"
#include "../../game-engine/GameEngine.h"
#include "../../lcd/LCD.h"
#include "../../utils/utils.h"

class GameStatus {
   public:
    LCD *lcd;

    GameStatus(LCD *lcd) {
        this->lcd = lcd;
    }

    void show(short score, short lifes, unsigned long startTime);
};

void GameStatus ::show(short score, short lifes, unsigned long startTime) {
    char scoreMessage[getNumberOfDigits(score) + strlen("Score: ") + 1];
    sprintf(scoreMessage, "Score: %d", score);

    this->lcd->printOnRow(scoreMessage, 0);

    char lifesMessage[getNumberOfDigits(lifes) + 1];

    sprintf(lifesMessage, "%d", lifes);

    // this->lcd->printOnRow("|", 0, strlen(scoreMessage) + 3);

    this->lcd->printOnRow(lifesMessage, 1);

    this->lcd->printOnRowAndColumn(heartChar, 1, strlen(lifesMessage) + 1, true);
}