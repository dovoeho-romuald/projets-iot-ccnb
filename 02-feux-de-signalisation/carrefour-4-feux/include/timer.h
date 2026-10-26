/*
  Titre      : Feu de signalisation
  Auteur     : Dovoeho Romuald Yemalin
  Date       : 04/02/2026
  Description: Projet de Feu de Signalisation Routier pour Véhicules avec Arduino et PlatformIO
  Version    : 0.0.2
*/

#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {
  private:
    unsigned long startTime;
    unsigned long duration;

  public:
    Timer();
    void startTimer(unsigned long d);
    bool isTimerReady();
};

#endif
