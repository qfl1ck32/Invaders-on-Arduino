#ifndef SettingsSoundsMenuState_h
#define SettingsSoundsMenuState_h

#include <avr/pgmspace.h>

#include "../../../state/State.h"

class SettingsSoundsMenuState : public State {
   public:
    SettingsSoundsMenuState(int8_t id) : State(id) {}

    void setup();
    void handle();
    void cleanup();

    static void goBack();

    static void increaseContrast();

    static void decreaseContrast();

    static void increaseBacklight();

    static void decreaseBacklight();

    static void switchMusicPlaying();

    static const char turnMusicOn[];
    static const char turnMusicOff[];

    static void setMessages(bool);
};

#endif
