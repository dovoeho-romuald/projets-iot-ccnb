#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "mbedtls/base64.h"
#include "DFRobot_RGBLCD1602.h"

DFRobot_RGBLCD1602 lcd(0x2D, 16, 2);
const char* WIFI_SSID = "VOTRE_WIFI";
const char* WIFI_PASS = "VOTRE_MOT_DE_PASSE";

const char* MQTT_SERVER = "nam1.cloud.thethings.network";
const int   MQTT_PORT   = 1883;
const char* MQTT_USER   = "VOTRE_UTILISATEUR";
const char* MQTT_PASS   = "VOTRE_MOT_DE_PASSE";
const char* MQTT_TOPIC  = "v3/projet-iot-ncrc1205@ttn/devices/projet-iot-ncrc1205/up";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* message, unsigned int length) {
  JsonDocument doc;
  if (deserializeJson(doc, message, length)) {
    Serial.println("Erreur JSON");
    return;
  }
  const char* frm = doc["uplink_message"]["frm_payload"];
  if (frm == nullptr) return;

  unsigned char b[8];
  size_t n = 0;
  mbedtls_base64_decode(b, sizeof(b), &n, (const unsigned char*)frm, strlen(frm));
  if (n < 3) return;

  float co = ((b[0] << 8) | b[1]) / 10.0;
  int mouvements = b[2];

  Serial.print("CO: "); Serial.print(co, 1);
  Serial.print(" | Mouvements: "); Serial.println(mouvements);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CO: "); lcd.print(co, 1);
  lcd.setCursor(0, 1);
  lcd.print("Mouvements: "); lcd.print(mouvements);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connexion a TTN...");
    if (client.connect("ESP32-LCD", MQTT_USER, MQTT_PASS)) {
      Serial.println("OK");
      client.subscribe(MQTT_TOPIC);
    } else {
      Serial.print("echec, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  lcd.init();
  
  lcd.print("Connexion WiFi");

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connecte");

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  client.setBufferSize(4096);

  lcd.clear();
  lcd.print("Attente TTN...");
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}