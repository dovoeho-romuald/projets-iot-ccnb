#include <WiFi.h>
#include <ArduinoMqttClient.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <Adafruit_SHT4x.h>

// WiFi
char ssid[] = "VOTRE_WIFI";
char pass[] = "VOTRE_MOT_DE_PASSE";

// Adafruit IO
const char broker[] = "io.adafruit.com";
int port = 1883;

const char username[] = "VOTRE_UTILISATEUR";
const char aio_key[]  = "VOTRE_CLE";

const char tempTopic[] = "Dovoeho/feeds/temperature";
const char humTopic[]  = "Dovoeho/feeds/humidity";

// ThingSpeak
WiFiClient client;
unsigned long channelNumber = 0000000;
const char * writeAPIKey = "VOTRE_CLE";

// MQTT
MqttClient mqttClient(client);

// Capteur
Adafruit_SHT4x sht4;

unsigned long previousMillis = 0;
const long interval = 20000;

void setup() {
  Serial.begin(115200);
  delay(2000);

  if (!sht4.begin()) {
    Serial.println("Erreur capteur");
    while (1);
  }

  Serial.println("Capteur OK");

  Serial.print("Connexion WiFi...");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connecté");

  ThingSpeak.begin(client);

  mqttClient.setUsernamePassword(username, aio_key);

  Serial.print("Connexion MQTT...");
  if (!mqttClient.connect(broker, port)) {
    Serial.println("Erreur MQTT");
    while (1);
  }

  Serial.println("Connecté AIO");
}

void loop() {
  mqttClient.poll();

  // Reconnexion MQTT si perdu
  if (!mqttClient.connected()) {
    Serial.println("MQTT reconnect...");
    if (mqttClient.connect(broker, port)) {
      Serial.println("Reconnecté AIO");
    } else {
      Serial.println("Echec MQTT");
    }
  }

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    sensors_event_t humidity, temp;
    sht4.getEvent(&humidity, &temp);

    float temperature = temp.temperature;
    float hum = humidity.relative_humidity;

    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print("  Hum: ");
    Serial.println(hum);

    // Envoi Adafruit IO
    mqttClient.beginMessage(tempTopic);
    mqttClient.print(temperature);
    mqttClient.endMessage();

    mqttClient.beginMessage(humTopic);
    mqttClient.print(hum);
    mqttClient.endMessage();

    Serial.println("Envoyé à Adafruit IO");

    // Envoi ThingSpeak
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, hum);

    int x = ThingSpeak.writeFields(channelNumber, writeAPIKey);

    if (x == 200) {
      Serial.println("Envoyé à ThingSpeak");
    } else {
      Serial.println("Erreur ThingSpeak");
    }
  }
}