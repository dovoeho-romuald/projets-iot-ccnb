#ifndef LUMIERE_H
#define LUMIERE_H

#include <Arduino.h>

class Lumiere {
  private:
    int pin;

  public:
    Lumiere(int p);
    int lire();
};

#endif
