/*
  ArduinoMqttClient - WiFi Simple Sender

  This example connects to a MQTT broker and publishes a message to
  a topic once a second.

  The circuit:
  - Arduino MKR 1000, MKR 1010 or Uno WiFi Rev2 board

  This example code is in the public domain.
*/
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>   
#include <DHT22.h>

#define pinDATA 2
#define pinMouv 4
#define buzzerPin 3

DHT22 dht22(pinDATA);
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "VOTRE_WIFI";    // your network SSID (name)
char pass[] = "VOTRE_MOT_DE_PASSE";    // your network password (use for WPA, or use as key for WEP)

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate 
//    flashed in the WiFi module.

const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "arduino/simple";

const long interval = 1000;
unsigned long previousMillis = 0;

int count = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  
  pinMode(pinMouv, INPUT);
  pinMode(buzzerPin, OUTPUT);

  
  Serial.print("Connexion WiFi...");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(2000);
  }

  Serial.println("\nConnecté au WiFi !");

  
  Serial.print("Connexion MQTT...");
  if (!mqttClient.connect(broker, port)) {
    Serial.print("Erreur MQTT: ");
    Serial.println(mqttClient.connectError());
    while (1);
  }

  Serial.println("Connecté au broker MQTT !");
}


void loop() {

  
  if (digitalRead(pinMouv) == HIGH) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }


  float t = dht22.getTemperature();
  float h = dht22.getHumidity();

  if (dht22.getLastError() != dht22.OK) {
    Serial.println("Erreur capteur DHT22");
  }

  Serial.print("Température: ");
  Serial.print(t);
  Serial.print(" °C | Humidité: ");
  Serial.println(h);

  
  mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    mqttClient.beginMessage(topic);
    mqttClient.print("Temp=");
    mqttClient.print(t);
    mqttClient.print(" Hum=");
    mqttClient.print(h);
    mqttClient.print(" Count=");
    mqttClient.print(count);
    mqttClient.endMessage();

    Serial.println("Message envoyé MQTT");

    count++;
  }

  delay(2000);
}
