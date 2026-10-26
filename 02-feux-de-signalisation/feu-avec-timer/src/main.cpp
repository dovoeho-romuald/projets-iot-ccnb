#include <Arduino.h>
#include "Timer.h"

// CLASSE FEU 
class Feu {
  private:
    int rouge;
    int jaune;
    int vert;

  public:
    Feu(int r, int j, int v) {
      rouge = r;
      jaune = j;
      vert  = v;
    }

    void init() {
      pinMode(rouge, OUTPUT);
      pinMode(jaune, OUTPUT);
      pinMode(vert, OUTPUT);
    }

    void vertOn() {
      digitalWrite(vert, HIGH);
      digitalWrite(jaune, LOW);
      digitalWrite(rouge, LOW);
    }

    void jauneOn() {
      digitalWrite(vert, LOW);
      digitalWrite(jaune, HIGH);
      digitalWrite(rouge, LOW);
    }

    void rougeOn() {
      digitalWrite(vert, LOW);
      digitalWrite(jaune, LOW);
      digitalWrite(rouge, HIGH);
    }
};

// FEUX
Feu feuA(1, 2, 3);
Feu feuB(8, 7, 6);
Feu feuC(4, 5, 14);
Feu feuD(13, 11, 12); 

Timer timer;

// ETATS DU CARREFOUR
enum Etat {
  FACE_VERT,
  FACE_JAUNE,
  COTE_VERT,
  COTE_JAUNE
};

Etat etat = FACE_VERT;

//SETUP
void setup() {
  feuA.init();
  feuB.init();
  feuC.init();
  feuD.init();

  timer.startTimer(5000);
}

// LOOP
void loop() {

  switch (etat) {

    case FACE_VERT:
      feuA.vertOn();
      feuB.vertOn();
      feuC.rougeOn();
      feuD.rougeOn();

      if (timer.isTimerReady()) {
        etat = FACE_JAUNE;
        timer.startTimer(2000);
      }
      break;

    case FACE_JAUNE:
      feuA.jauneOn();
      feuB.jauneOn();

      if (timer.isTimerReady()) {
        feuA.rougeOn();
        feuB.rougeOn();
        etat = COTE_VERT;
        timer.startTimer(5000);
      }
      break;

    case COTE_VERT:
      feuC.vertOn();
      feuD.vertOn();

      if (timer.isTimerReady()) {
        etat = COTE_JAUNE;
        timer.startTimer(2000);
      }
      break;

    case COTE_JAUNE:
      feuC.jauneOn();
      feuD.jauneOn();

      if (timer.isTimerReady()) {
        feuC.rougeOn();
        feuD.rougeOn();
        etat = FACE_VERT;
        timer.startTimer(5000);
      }
      break;
  }
}
