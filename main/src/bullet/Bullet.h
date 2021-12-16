#ifndef Bullet_h
#define Bullet_h

#include "../constants/app.h"
#include "../delayer/Delayer.h"
#include "../pixel/Pixel.h"
#include "../unit/Unit.h"

class Bullet : public Unit {
   public:
    Bullet(byte, byte);

    Delayer delayer = Delayer(250);

    Bullet() {}

    void action();

    void behaviour(byte);

    unsigned char getType();
};

#endif