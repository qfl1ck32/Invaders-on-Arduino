#include "./globals.h"

#include "../constants/app.h"

StateManager stateManager = StateManager();

LCD *lcd = new LCD(RS, enable, d4, d5, d6, d7, contrast, backlight);

Joystick joystick = Joystick(pinSW, pinX, pinY);

NameSelector *nameSelector = new NameSelector(lcd, 1);

Menu menu = Menu(lcd);

Button button = Button(buttonPin);

Matrix *matrix = new Matrix(dinPin, clockPin, loadPin, 1, matrixRows, matrixCols);

Leaderboard leaderboard = Leaderboard();

Buzzer *songBuzzer = new Buzzer(songBuzzerPin);
Buzzer *gameSoundsBuzzer = new Buzzer(gameSoundsBuzzerPin);

GameEngine *gameEngine = new GameEngine(MATRIX_ROWS, MATRIX_COLUMNS);

GraphicsEngine graphicsEngine = GraphicsEngine(matrix);

GameStatus gameStatus = GameStatus(lcd);

Game game = Game();

void menuGoUp() {
    menu.goUp();
}

void menuGoDown() {
    menu.goDown();
}

void menuSelect() {
    menu.select();
}

void initialiseRandomSeed() {
    // FIXME: 0 is used because it's not used
    randomSeed(analogRead(0));
}

bool getUsesMusic() {
    byte value = EEPROM.read(EEPROM_MUSIC_PLAYING_INDEX);

    // TODO: "true" is the default value
    return value == 255 ? true : value == 1;
}

bool usesMusic = getUsesMusic();
