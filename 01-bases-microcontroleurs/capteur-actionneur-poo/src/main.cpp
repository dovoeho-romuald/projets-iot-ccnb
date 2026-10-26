#include <Arduino.h>
#include "Led.h"
#include "Lumiere.h"

Led maLed(3);
Lumiere capteur(A0);

void setup(){
  Serial.begin(115200);
}

void loop(){
  int valeur = capteur.lire();

  if(valeur < 500){
    maLed.blink();
  } else {
    maLed.off();
  }
}
