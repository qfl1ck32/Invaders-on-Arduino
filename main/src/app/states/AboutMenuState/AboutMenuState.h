#ifndef AboutMenuState_h
#define AboutMenuState_h

#include <avr/pgmspace.h>

#include "../../../state/State.h"

class AboutMenuState : public State {
   public:
    AboutMenuState(int8_t id) : State(id) {}

    void setup();
    void handle();
    void cleanup();

    static void goBack();
};

#endif
