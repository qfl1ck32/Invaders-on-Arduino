#include "./Egg.h"

uint8_t Egg::fallSpeed = EGG_INITIAL_FALL_DELAYER_INTERVAL;

Egg ::Egg(int8_t x, int8_t y) : Unit(x, y) {
    this->delayer.updateInterval(Egg::fallSpeed);
}

void Egg::action() {
    if (!this->delayer.canRun()) return;

    Unit *unit = this->engine->unitMatrix[this->x + 1][this->y];

    if (unit != 0 && unit->getType() == SPACESHIP_TYPE) {
        this->sendMessage(KILL, *unit);
        this->die();
        return;
    }

    if (this->x == this->engine->rows - 2) {
        this->die();
        return;
    }

    this->move(1, 0);
};

void Egg::behaviour(int8_t action) {
    switch (action) {
        case KILL:
            this->die();
            break;
    }
};

unsigned char Egg::getType() {
    return EGG_TYPE;
}