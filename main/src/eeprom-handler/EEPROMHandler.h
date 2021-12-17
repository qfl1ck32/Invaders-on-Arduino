#ifndef EEPROMHandler_h
#define EEPROMHandler_h

#include <EEPROM.h>

#include "Arduino.h"

class EEPROMHandler {
   public:
    short startAt, limit, writeIndex, readIndex;

    EEPROMHandler(int startAt, int limit) {
        this->startAt = startAt;
        this->limit = limit;

        this->readIndex = startAt;

        for (int i = this->startAt; i < this->limit; ++i) {
            if (EEPROM.read(i) == 255) {
                this->writeIndex = i;
                break;
            }
        }
    }

    char* readString(int);

    char* readNext();

    void writeString(const char*, int, int);
    void writeString(const char*, int);

    void resetReadHead();

    void clear();

    int8_t read(int);
};

#endif
