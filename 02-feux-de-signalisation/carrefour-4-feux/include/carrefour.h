/*
  Titre      : Feu de signalisation
  Auteur     : Dovoeho Romuald Yemalin
  Date       : 04/02/2026
  Description: Projet de Feu de Signalisation Routier pour Véhicules avec Arduino et PlatformIO
  Version    : 0.0.2
*/

#ifndef CARREFOUR_H
#define CARREFOUR_H

#include <Arduino.h>
#include "Feu.h"
#include "Timer.h"

class Carrefour {

  private:
    // Références vers les feux
    Feu &feuA;
    Feu &feuB;
    Feu &feuC;
    Feu &feuD;

    Timer timer;

    enum Etat{
      FACE_VERT,
      FACE_VERT_CLIGNO,
      FACE_JAUNE,
      COTE_VERT,
      COTE_VERT_CLIGNO,
      COTE_JAUNE
    };

    Etat etat;

    bool blinkState;
    unsigned long lastBlink;

  public:

    // Constructeur sans abréviations
    Carrefour(
      Feu &feuFace1,
      Feu &feuFace2,
      Feu &feuCote1,
      Feu &feuCote2
    );

    void update();
};

#endif
