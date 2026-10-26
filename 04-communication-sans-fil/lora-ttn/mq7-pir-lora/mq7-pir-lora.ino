
#include <MKRWAN.h>

LoRaModem modem;

#include "arduino_secrets.h"

String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

const int PIN_MQ7 = A1;   
const int PIN_PIR = 7;    


const int N = 10;
int lectures[N];
int indexLecture = 0;
long somme = 0;


int etatPrecedentPIR = LOW;
int compteurMouvements = 0;


const unsigned long INTERVALLE_LECTURE = 1000;   // 
const unsigned long INTERVALLE_ENVOI   = 60000;  
unsigned long derniereLecture = 0;
unsigned long dernierEnvoi = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(PIN_PIR, INPUT);
  analogReadResolution(10);   // 0 à 1023

 
  int v = analogRead(PIN_MQ7);
  for (int i = 0; i < N; i++) { lectures[i] = v; somme += v; }

  
  if (!modem.begin(US915)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  
  for (int i = 0; i < 72; i++) modem.disableChannel(i);
  for (int i = 8; i <= 15; i++) modem.enableChannel(i);
  modem.enableChannel(65);

 
  int connected = modem.joinOTAA(appEui, appKey);
  while (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    delay(10000);
    Serial.println("Nouvel essai de join...");
    connected = modem.joinOTAA(appEui, appKey);
  }
  Serial.println("Connecte a TTN !");

  modem.minPollInterval(60);
  dernierEnvoi = millis();
}

void loop() {
  
  int etatPIR = digitalRead(PIN_PIR);
  if (etatPIR == HIGH && etatPrecedentPIR == LOW) {
    compteurMouvements++;
    Serial.println(">>> Mouvement detecte !");
  }
  etatPrecedentPIR = etatPIR;

  
  if (millis() - derniereLecture >= INTERVALLE_LECTURE) {
    derniereLecture = millis();
    int brut = analogRead(PIN_MQ7);
    somme = somme - lectures[indexLecture] + brut;  
    lectures[indexLecture] = brut;
    indexLecture = (indexLecture + 1) % N;
  }


  if (millis() - dernierEnvoi >= INTERVALLE_ENVOI) {
    dernierEnvoi = millis();

    float moyenneCO = somme / (float)N;
    uint16_t co10 = (uint16_t)(moyenneCO * 10);               
    uint8_t mouvements = (compteurMouvements > 255) ? 255 : compteurMouvements;

    byte payload[3];
    payload[0] = highByte(co10);
    payload[1] = lowByte(co10);
    payload[2] = mouvements;

    Serial.print("Envoi -> CO moyen: "); Serial.print(moyenneCO, 1);
    Serial.print(" | mouvements: ");     Serial.println(mouvements);

    modem.beginPacket();
    modem.write(payload, sizeof(payload));
    int err = modem.endPacket(false);   
    if (err > 0) {
      Serial.println("Message sent correctly!");
      compteurMouvements = 0;          
    } else {
      Serial.println("Error sending message :(");
    }
  }
}