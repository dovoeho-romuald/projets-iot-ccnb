
               //SUPER MARIO + LCD + CAPTEUR MAGNETIQUE


// MUSIQUE MARIO 
#define NOTE_E4  330
#define NOTE_G4  392
#define NOTE_C4  262
#define NOTE_G3  196
#define NOTE_E3  165
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_AS3 233
#define NOTE_F4  349
#define NOTE_D4  294
#define NOTE_C5  523
// (je peux garder les autres notes si je voulais toute la musique)

int melody[] = {
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_C4, NOTE_E4, NOTE_G4, NOTE_G3
};

int noteDuration[] = {8, 4, 4, 8, 4, 2, 2};

int buzzerPin = 8;

//CAPTEUR MAGNETIQUE 
int capteurPin = 3;
int valeurCapteur = 0;

// ----------- LCD RGB -----------
#include "DFRobot_RGBLCD1602.h"
DFRobot_RGBLCD1602 lcd(0x60, 16, 2);

//definition de couleur de l'ecran et l'action du buzzer mario

void setup() {
  Serial.begin(115200);

  pinMode(capteurPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.init();
  lcd.setRGB(0, 50, 255);   // bleu par défaut
  lcd.print("Pret...");
}

// -----------------------------------------------------

void jouerMario() {
  for (int i = 0; i < 7; i++) {
    int duration = 800 / noteDuration[i];
    tone(buzzerPin, melody[i], duration);
    delay(duration * 1.3);
    noTone(buzzerPin);
  }
}

// -----------------------------------------------------

void loop() {

  valeurCapteur = digitalRead(capteurPin);

  if (valeurCapteur == HIGH) {  
    // Aimant proche (la plupart des capteurs donnent LOW quand activé)
    lcd.clear();
    lcd.setCursor(0, 0);        //colonne 0, ligne 0 equivalent de la ligne 1
    lcd.print("ALerte Romuald");

    lcd.setCursor(0, 1);       //colonne 1, ligne 1 equivalent de la ligne 2
    lcd.print("Aimant detecter!");
    lcd.setRGB(255, 0, 0); // rouge

    jouerMario();
    delay(500);
  }
  else {
    lcd.clear();
    lcd.print("Rien detecte");
    lcd.setRGB(0, 255, 0); // vert
  }

  delay(200);
}

