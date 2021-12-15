#include "./GameEngine.h"

GameEngine::GameEngine(byte rows, byte columns) {
    this->rows = rows;
    this->columns = columns;

    this->unitMatrix = new Unit**[rows];

    for (int i = 0; i < rows; ++i) {
        this->unitMatrix[i] = new Unit*[columns];
    }

    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->columns; ++j) {
            this->unitMatrix[i][j] = 0;
        }
    }

    for (int i = 0; i < MAX_UNITS; ++i) {
        this->unitArray[i] = 0;
    }

    this->numberOfUnits = 0;

    this->pixelChanges = new LinkedList<PixelChange>();
}

void GameEngine::resetState() {
    this->numberOfUnits = 0;

    // TODO: why
    for (int i = 0; i < MAX_UNITS; ++i) {
        if (this->unitArray[i] != 0) {
            delete this->unitArray[i];
        }
    }

    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->columns; ++j) {
            this->unitMatrix[i][j] = 0;
        }
    }

    for (int i = 0; i < MAX_UNITS; ++i) {
        this->unitArray[i] = 0;
    }
}

void GameEngine::run() {
    for (int i = 0; i < this->numberOfUnits; ++i) {
        this->unitArray[i]->action();
    }

    for (int i = 0; i < this->numberOfUnits; ++i) {
        this->unitArray[i]->react();
    }

    for (int i = 0; i < this->numberOfUnits; ++i) {
        if (this->unitArray[i]->isAlive) {
            continue;
        }

        byte x = this->unitArray[i]->x;
        byte y = this->unitArray[i]->y;

        this->unitMatrix[x][y] = 0;

        delete this->unitArray[i];

        this->unitArray[i] = 0;

        for (int j = i + 1; j < this->numberOfUnits; ++j) {
            this->unitArray[j - 1] = this->unitArray[j];
        }

        this->pixelChanges->add(PixelChange(x, y, false));

        --this->numberOfUnits;
        --i;
    }
}

bool GameEngine::isValidPosition(byte x, byte y) {
    return x >= 0 && y >= 0 && x < this->rows && y < this->columns;
}