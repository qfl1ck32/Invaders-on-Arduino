#include "Arduino.h"

#define EEPROM_MATRIX_INTENSITY_INDEX 0
#define EEPROM_LCD_BACKLIGHT_INDEX 1
#define EEPROM_MUSIC_PLAYING_INDEX 2
#define EEPROM_LCD_CONTRAST_INDEX 3

#define EEPROM_LEADERBOARD_START_INDEX 4

#define EEPROM_MISSING_VALUE 255

#define MAX_MESSAGES 5

#define BULLET_TYPE 0
#define CHICKEN_TYPE 1
#define EGG_TYPE 2
#define SPACESHIP_TYPE 3

#define KILL 1

#define MATRIX_ROWS 8
#define MATRIX_COLUMNS 8

extern const byte heartCharArray[8];
extern byte heartChar;

#define MATRIX_ROWS 8
#define MATRIX_COLUMNS 8

#define CHICKEN_INITIAL_EGG_DELAYER_INTERVAL 2000
#define CHICKEN_INITIAL_MOVE_DELAYER_INTERVAL 1000
#define EGG_INITIAL_FALL_DELAYER_INTERVAL 250

#define CHICKEN_DELAYER_INTERVAL_DECREASE_FACTOR 50
#define EGG_DELAYER_INTERVAL_DECREASE_FACTOR 15

#define REMAINING_LIFES_SCORE_FACTOR 8

#define USED_BULLETS_INITIAL_BONUS 11

#define INT_MAX 0x7FFF

#define MIN_LEVEL 1
#define MAX_LEVEL 10

// TODO: add constants from globals, here.

extern const uint64_t matrixNumbers[];
