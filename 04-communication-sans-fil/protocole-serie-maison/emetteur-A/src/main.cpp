#include <Arduino.h>

#define PIN_GAZ 19
#define PIN_MOUVEMENT 18
#define PIN_AIMANT 5
#define PIN_TX 23

#define BIT_DURATION 1000
#define START_DURATION 2000

byte calculateParity(byte data) {
  byte count = 0;
  for (int i = 0; i < 8; i++) {
    if (bitRead(data, i)) count++;
  }
  return (count % 2 == 0) ? 1 : 0;
}

void sendFrame(byte data, byte parity) {

  // Signal début
  digitalWrite(PIN_TX, HIGH);
  delayMicroseconds(START_DURATION);

  // Envoi 8 bits
  for (int i = 0; i < 8; i++) {
    digitalWrite(PIN_TX, bitRead(data, i));
    delayMicroseconds(BIT_DURATION);
  }

  // Parité
  digitalWrite(PIN_TX, parity);
  delayMicroseconds(BIT_DURATION);

  // Fin
  digitalWrite(PIN_TX, LOW);
}

void setup() {
  pinMode(PIN_GAZ, INPUT);
  pinMode(PIN_MOUVEMENT, INPUT);
  pinMode(PIN_AIMANT, INPUT);
  pinMode(PIN_TX, OUTPUT);

  digitalWrite(PIN_TX, LOW);
  Serial.begin(115200);
}

void loop() {

  byte gaz = digitalRead(PIN_GAZ);
  byte mouvement = digitalRead(PIN_MOUVEMENT);
  byte aimant = digitalRead(PIN_AIMANT);

  byte data = 0;
  bitWrite(data, 0, gaz);
  bitWrite(data, 1, mouvement);
  bitWrite(data, 2, aimant);

  byte parity = calculateParity(data);

  sendFrame(data, parity);

  Serial.print("Envoye: ");
  Serial.print(gaz);
  Serial.print(mouvement);
  Serial.println(aimant);

  delay(300);
}
