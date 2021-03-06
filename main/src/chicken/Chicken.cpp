#include "./Chicken.h"

#include "../app/globals.h"

int8_t Chicken::count = 0;

Chicken::Chicken(int8_t x, int8_t y) : Unit(x, y) {
    ++Chicken::count;

    this->eggDelayer.updateInterval(CHICKEN_INITIAL_EGG_DELAYER_INTERVAL);
    this->moveDelayer.updateInterval(CHICKEN_INITIAL_MOVE_DELAYER_INTERVAL);
}

void Chicken::action() {
    static byte maxRow = 2;

    if (this->eggDelayer.canRun()) {
        new Egg(this->x + 1, this->y);
    }

    if (!this->moveDelayer.canRun()) return;

    short rand = random(3);
    short rand2 = random(3);

    short xDirection = rand ? (rand & 1 ? 1 : -1) : 0;
    short yDirection = rand2 ? (rand2 & 1 ? 1 : -1) : 0;

    if (this->x + xDirection >= maxRow) return;

    if (!this->engine->isValidPosition(this->x + xDirection, this->y + yDirection) || this->engine->unitMatrix[this->x + xDirection][this->y + yDirection] != 0) {
        return;
    }

    this->move(xDirection, yDirection);
};

Chicken::~Chicken() {
    --Chicken::count;
}

void Chicken::die() {
    Unit::die();
}

void Chicken::behaviour(int8_t action) {
    switch (action) {
        case KILL:
            this->die();

            return;
    }
};

unsigned char Chicken::getType() {
    return CHICKEN_TYPE;
}
