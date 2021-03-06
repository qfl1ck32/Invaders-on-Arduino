#include "./PlayingState.h"

#include "../../globals.h"

int8_t PlayingState::maxLevel = 5;
uint8_t PlayingState::state = NOT_INITIALISED;
uint8_t PlayingState::previousState = NOT_INITIALISED;
short PlayingState::score = 0;
uint8_t PlayingState::maxSecondsPerLevel = 30;

Leaderboard* PlayingState::leaderboard = 0;
Spaceship* PlayingState::spaceship = 0;

PlayingState::PlayingState() {
    this->graphicsEngine = new GraphicsEngine(matrix);
    this->gameStatus = new GameStatus(lcd);
    this->scrollDelayer.updateInterval(350);

    PlayingState::leaderboard = new Leaderboard();
}

PlayingState::~PlayingState() {
    delete this->graphicsEngine;
    delete this->gameStatus;
    delete PlayingState::leaderboard;
}

void PlayingState::setup() {
    Unit::engine = new GameEngine(MATRIX_ROWS, MATRIX_COLUMNS);

    this->reset();

    this->setupLevel();

    lcd->shouldClearRowOnPrint = false;
}

void PlayingState::handle() {
    if (PlayingState::previousState != PlayingState::state) {
        if (PlayingState::previousState == WON) {
            this->setupLevel();
        }

        PlayingState::previousState = state;
    }

    switch (PlayingState::state) {
        case PLAYING:
            this->play();
            break;
        case NOT_INITIALISED:
            break;
        default:
            this->handleNotPlayingState();
            break;
    }
}

void PlayingState::reset() {
    PlayingState::score = 0;

    setLevel(settingsLevel);

    PlayingState::updateState(NOT_INITIALISED);

    if (PlayingState::spaceship != 0) {
        PlayingState::spaceship = 0;
        delete PlayingState::spaceship;
    }
}

void PlayingState::play() {
    joystick.handleJoystickMovements();

    Unit::engine->run();

    int timeSoFar = this->getTimeSoFar();

    int secondsLeft = this->maxSecondsPerLevel - timeSoFar;

    if (secondsLeft == 0) {
        this->switchToLostState();
        return;
    }

    this->gameStatus->show(PlayingState::score, PlayingState::spaceship->lifes, level, secondsLeft);
    this->graphicsEngine->renderChanges(Unit::engine->pixelChanges);

    if (PlayingState::spaceship->lifes == 0) {
        this->switchToLostState();
        return;
    }

    if (Chicken::count != this->lastNumberOfChickens) {
        this->updateScore();
        this->lastNumberOfChickens = Chicken::count;

        if (Chicken::count == 0) {
            this->switchToWonState();
        }
    }
}

uint8_t PlayingState::getTimeSoFar() {
    return (int)((millis() - this->timeSinceLastStart) / 1000);
}

void PlayingState::switchToLostState() {
    static const uint64_t icon PROGMEM = 0x00423c0024242400;

    PlayingState::state = LOST;

    int numberOfDigitsInScore = getNumberOfDigits(PlayingState::score);

    char gameOverMessage[21 + numberOfDigitsInScore];

    static const char gameOver[] PROGMEM = "Game over. Score: ";
    static const char pressXToContinue[] PROGMEM = "Press X to continue.";

    char* gameOverMsg = readStringFromPROGMEM(gameOver);
    char* pressXToContinueMsg = readStringFromPROGMEM(pressXToContinue);

    sprintf(gameOverMessage, "%s%d.", gameOverMsg, PlayingState::score);

    lcd->printOnRow(gameOverMessage, 0);
    lcd->printOnRow(pressXToContinueMsg, 1);

    delete gameOverMsg;
    delete pressXToContinueMsg;

    button.setOnStateChange(PlayingState::goToNextScreenAfterDeath);

    uint64_t image = readImageFromPROGMEM(&icon);

    matrix->displayImage(image);
}

void PlayingState::goToNextScreenAfterDeath() {
    if (PlayingState::leaderboard->isHighScore(PlayingState::score)) {
        stateManager.changeState<NameSelectionState>();
    } else {
        stateManager.changeState<MainMenuState>();
    }
}

void PlayingState::switchToWonState() {
    static const uint64_t icon PROGMEM = 0x003c420024242400;

    static const char youWonLvl[] PROGMEM = "You won level ";

    PlayingState::state = WON;

    short numberOfDigitsInLevel = getNumberOfDigits(level);

    char youWon[15 + numberOfDigitsInLevel];

    char* youWonLevel = readStringFromPROGMEM(youWonLvl);

    sprintf(youWon, "%s%d!", youWonLevel, level);

    char* pressXToContinue = readStringFromPROGMEM(pressXToContinueMessage);

    lcd->printOnRow(youWon, 0);
    lcd->printOnRow(pressXToContinue, 1);

    delete youWonLevel;
    delete pressXToContinue;

    button.setOnStateChange(PlayingState::goToNextLevel);

    uint64_t image = readImageFromPROGMEM(&icon);

    matrix->displayImage(image);
}

void PlayingState::handleNotPlayingState() {
    if (this->scrollDelayer.canRun()) {
        lcd->scrollRow(0);
        lcd->scrollRow(1);
    }
}

void PlayingState::updateScore() {
    PlayingState::score += (this->maxSecondsPerLevel - this->getTimeSoFar()) / 3;

    if (Chicken::count == 0) {
        byte shotBullets = Bullet::numberOfBulletsShotSoFar;

        byte shotsBonus = USED_BULLETS_INITIAL_BONUS - shotBullets;

        if (shotsBonus > 0) {
            PlayingState::score += shotsBonus;
        }

        PlayingState::score += REMAINING_LIFES_SCORE_FACTOR * spaceship->lifes;

        Bullet::numberOfBulletsShotSoFar = 0;
    }
}

void PlayingState::setupLevel() {
    joystick.setOnChangeUp(PlayingState::moveUp);
    joystick.setOnChangeDown(PlayingState::moveDown);

    joystick.setOnChangeLeft(PlayingState::moveLeft);
    joystick.setOnChangeRight(PlayingState::moveRight);

    button.setOnStateChange(PlayingState::attack);

    lcd->clear();

    this->gameStatus->displayHeader();

    matrix->setAllLeds(false);

    static int8_t spaceshipDefaultX = 7;
    static int8_t spaceshipDefaultY = 2;

    this->timeSinceLastStart = millis();

    if (PlayingState::state == NOT_INITIALISED) {
        if (PlayingState::spaceship == 0) {
            PlayingState::spaceship = new Spaceship(spaceshipDefaultX, spaceshipDefaultY);
        }
    }

    else {
        spaceship->move(spaceshipDefaultX - spaceship->x, spaceshipDefaultY - spaceship->y);

        Unit::engine->pixelChanges->add(PixelChange(spaceship->x, spaceship->y, true));
        this->graphicsEngine->renderChanges(Unit::engine->pixelChanges);
    }

    Chicken* chicken = new Chicken(0, random(Unit::engine->columns / 2));

    chicken->eggDelayer.updateInterval(CHICKEN_INITIAL_EGG_DELAYER_INTERVAL - CHICKEN_DELAYER_INTERVAL_DECREASE_FACTOR * level);
    chicken->moveDelayer.updateInterval(CHICKEN_INITIAL_MOVE_DELAYER_INTERVAL - CHICKEN_DELAYER_INTERVAL_DECREASE_FACTOR * level);

    Egg::fallSpeed = EGG_INITIAL_FALL_DELAYER_INTERVAL - EGG_DELAYER_INTERVAL_DECREASE_FACTOR * level;

    this->lastNumberOfChickens = Chicken::count;

    PlayingState::state = PLAYING;
}

void PlayingState::goToNextLevel() {
    if (level == MAX_LEVEL) {
        if (PlayingState::leaderboard->isHighScore(PlayingState::score)) {
            stateManager.changeState<NameSelectionState>();
        } else {
            stateManager.changeState<MainMenuState>();
        }

        setLevel(MIN_LEVEL);

        return;
    }

    setLevel(level + 1);

    spaceship->lifes += 1;

    for (uint8_t i = 0; i < Unit::engine->numberOfUnits; ++i) {
        Unit* unit = Unit::engine->unitArray[i];

        unsigned char type = unit->getType();

        if (type == CHICKEN_TYPE || type == EGG_TYPE || type == BULLET_TYPE) {
            unit->die();
        }
    }

    Unit::engine->run();

    PlayingState::updateState(PLAYING);
}

void PlayingState::cleanup() {
    lcd->clear();
    matrix->setAllLeds(false);

    joystick.clearHandlers();

    lcd->shouldClearRowOnPrint = true;

    delete Unit::engine;
}

void PlayingState::moveUp() {
    PlayingState::spaceship->move(-1, 0);
}

void PlayingState::moveRight() {
    PlayingState::spaceship->move(0, 1);
}

void PlayingState::moveDown() {
    PlayingState::spaceship->move(1, 0);
}

void PlayingState::moveLeft() {
    PlayingState::spaceship->move(0, -1);
}

void PlayingState::attack() {
    PlayingState::spaceship->attack();
}

void PlayingState::updateState(uint8_t newState) {
    PlayingState::previousState = PlayingState::state;
    PlayingState::state = newState;
}
