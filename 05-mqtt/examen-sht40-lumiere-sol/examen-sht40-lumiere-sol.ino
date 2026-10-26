// Examen-MQTT.ino

#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_NICLA_VISION) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_GIGA) || defined(ARDUINO_OPTA)
  #include <WiFi.h>
#elif defined(ARDUINO_PORTENTA_C33)
  #include <WiFiC3.h>
#elif defined(ARDUINO_UNOR4_WIFI)
  #include <WiFiS3.h>
#endif

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "VOTRE_WIFI";    // your network SSID (name)
char pass[] = "VOTRE_MOT_DE_PASSE";    // your network password (use for WPA, or use as key for WEP)

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate 
//    flashed in the WiFi module.

#include <Wire.h>
#include <Adafruit_SHT4x.h>

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "192.168.10.203";
int        port     = 1883;
const char topic1[]  = "examen/Romuald/lumiere";
const char topic2[]  = "examen/Romuald/humidite_sol";
const char topic3[]  = "examen/Romuald/sht/temperature";
const char topic4[]  = "examen/Romuald/sht/humidity";

#define LDR_PIN A0
#define SOIL_PIN A1

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

const long interval = 1000;
unsigned long previousMillis = 0;

int count = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to WiFi network:
  Serial.print("Atempting to connect to WPA SSID : ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(2000);
  }

  Serial.print("You're connected to the wifi network : ");
  Serial.println(ssid);
  Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("username", "password");

  Serial.print("Atempting to connect to the MQTT broker : ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connexion failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  if (!sht4.begin()) {
    Serial.println("Erreur SHT");
    while (1);
  }
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  mqttClient.poll();

  // to avoid having delays in loop, we'll use the strategy from BlinkWithoutDelay
  // see: File -> Examples -> 02.Digital -> BlinkWithoutDelay for more info
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    int lumiere = analogRead(LDR_PIN);
    int humidite_sol = analogRead(SOIL_PIN);

    sensors_event_t humidity, temp;
    sht4.getEvent(&humidity, &temp);

    float temperature = temp.temperature;
    float hum = humidity.relative_humidity;

    Serial.print("Sending message to topic: ");
    Serial.println(topic1);
    mqttClient.beginMessage(topic1);
    mqttClient.print(lumiere);
    mqttClient.endMessage();

    Serial.print("Sending message to topic: ");
    Serial.println(topic2);
    mqttClient.beginMessage(topic2);
    mqttClient.print(humidite_sol);
    mqttClient.endMessage();

    Serial.print("Sending message to topic: ");
    Serial.println(topic3);
    mqttClient.beginMessage(topic3);
    mqttClient.print(temperature);
    mqttClient.endMessage();

    Serial.print("Sending message to topic: ");
    Serial.println(topic4);
    mqttClient.beginMessage(topic4);
    mqttClient.print(hum);
    mqttClient.endMessage();

    Serial.println();

    count++;
  }
}