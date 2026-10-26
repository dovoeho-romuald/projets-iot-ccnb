/*
  Titre      : Feu de signalisation
  Auteur     : Dovoeho Romuald Yemalin
  Date       : 04/02/2026
  Description: Projet de Feu de Signalisation Routier pour Véhicules avec Arduino et PlatformIO
  Version    : 0.0.2
*/

#ifndef FEU_H
#define FEU_H

#include <Arduino.h>

class Feu {
  private:
    int rouge;
    int jaune;
    int vert;

  public:
    Feu(int r,int j,int v);

    void init();
    void vertOn();
    void jauneOn();
    void rougeOn();
    void vertClignote(bool etat);
};

#endif
