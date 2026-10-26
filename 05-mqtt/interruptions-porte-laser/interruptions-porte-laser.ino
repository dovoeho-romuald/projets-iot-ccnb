/*
  Labo 1 - ELTE1079 : interruptions (porte + laser) et envoi MQTT
  Carte : Arduino MKR WiFi 1010

  Sources (Fichier > Exemples) :
   [A] WiFiNINA > ConnectWithWPA           -> connexion WiFi
   [B] ArduinoMqttClient > WiFiSimpleSender -> connexion et envoi MQTT
   [C] 02.Digital > BlinkWithoutDelay / Debounce -> millis() au lieu de delay()
   [D] Reference attachInterrupt()          -> interruptions
*/

// ---- [B] inclusions (WiFiSimpleSender) ----
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

// ---- [A] identifiants dans l'onglet arduino_secrets.h (ConnectWithWPA) ----
#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// ---- [B] client MQTT (WiFiSimpleSender) ----
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";   // remplacer par le serveur de l'enseignant
int        port     = 1883;

// *** Changer "prenom" pour votre prenom ***
const char topicPorte[] = "elte1079/lab1/prenom/porte";
const char topicLaser[] = "elte1079/lab1/prenom/laser";

// Broches avec interruption sur le MKR WiFi 1010
const int PIN_PORTE = 0;
const int PIN_LASER = 1;

// Niveau lu quand le faisceau est recu (ajuster selon le test : HIGH ou LOW)
const int LASER_DETECTE = HIGH;

// ---- [C] anti-rebond avec millis() (Debounce / BlinkWithoutDelay) ----
const unsigned long debounceDelay = 30;

// ---- [D] drapeaux modifies par les ISR : volatile ----
volatile bool changementPorte = false;
volatile bool changementLaser = false;
volatile unsigned long dernierChangementPorte = 0;

// [D] ISR : tres courtes, elles levent seulement un drapeau
void isrPorte() {
  dernierChangementPorte = millis();
  changementPorte = true;
}

void isrLaser() {
  changementLaser = true;
}

// [A] connexion WiFi (boucle de ConnectWithWPA)
void setupWifi() {
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println("You're connected to the network");
  Serial.println();
}

// [B] connexion MQTT (WiFiSimpleSender)
void setupMqtt() {
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  while (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    delay(5000);
  }
  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

// [B] envoi : beginMessage / print / endMessage (WiFiSimpleSender)
void publierPorte() {
  int etat = (digitalRead(PIN_PORTE) == HIGH) ? 1 : 0;   // 1 = ouverte
  mqttClient.beginMessage(topicPorte, true, 0);          // retain = true
  mqttClient.print(etat);
  mqttClient.endMessage();
  Serial.print("Porte : ");
  Serial.println(etat);
}

void publierLaser() {
  int etat = (digitalRead(PIN_LASER) == LASER_DETECTE) ? 1 : 0;   // 1 = detecte
  mqttClient.beginMessage(topicLaser, true, 0);
  mqttClient.print(etat);
  mqttClient.endMessage();
  Serial.print("Laser : ");
  Serial.println(etat);
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // attend le port serie
  }

  pinMode(PIN_PORTE, INPUT_PULLUP);   // reed entre la broche et GND
  pinMode(PIN_LASER, INPUT);          // diviseur 10k / 3,3k a l'exterieur

  setupWifi();
  setupMqtt();

  // [D] interruption sur tout changement d'etat
  attachInterrupt(digitalPinToInterrupt(PIN_PORTE), isrPorte, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_LASER), isrLaser, CHANGE);

  publierPorte();   // etat initial
  publierLaser();
}

void loop() {
  // [B] poll() garde la connexion MQTT active
  mqttClient.poll();

  if (WiFi.status() != WL_CONNECTED) setupWifi();
  if (!mqttClient.connected())       setupMqtt();

  // Porte : envoi apres la fin des rebonds
  if (changementPorte && (millis() - dernierChangementPorte >= debounceDelay)) {
    changementPorte = false;
    publierPorte();
  }

  // Laser : envoi immediat
  if (changementLaser) {
    changementLaser = false;
    publierLaser();
  }
}
