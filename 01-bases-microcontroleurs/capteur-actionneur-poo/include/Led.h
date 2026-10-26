#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led {
  private:
    int pin;

  public:
    Led(int p);
    void on();
    void off();
    void blink();
};

#endif
