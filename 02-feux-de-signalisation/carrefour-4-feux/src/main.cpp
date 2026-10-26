/*
  Titre      : Feu de signalisation
  Auteur     : Dovoeho Romuald Yemalin
  Date       : 04/02/2026
  Description: Projet de Feu de Signalisation Routier pour Véhicules avec Arduino et PlatformIO
  Version    : 0.0.2
*/
#include <Arduino.h>
#include "Feu.h"
#include "Carrefour.h"

Feu feuA(1,2,3);
Feu feuB(8,7,6);
Feu feuC(4,5,14);
Feu feuD(13,11,12);

Carrefour carrefour(feuA,feuB,feuC,feuD);

void setup(){
  feuA.init();
  feuB.init();
  feuC.init();
  feuD.init();
}

void loop(){
  carrefour.update();
}
