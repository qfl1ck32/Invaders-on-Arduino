#include "./app.h"

const byte heartCharArray[8] PROGMEM = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000,
    0b00000};

byte heartChar = 1;

const uint64_t matrixNumbers[] PROGMEM = {
    0x0010101014181000,
    0x00003c0810243800,
    0x00003c203c203c00,
    0x001010101c040400,
    0x00003c203c043c00,
};
