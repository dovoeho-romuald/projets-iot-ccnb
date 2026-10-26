// Publie {"temperature": ..., "humidity": ...} sur le topic data_dht_22
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>

// ===== A MODIFIER =====
const char* WIFI_SSID     = "VOTRE_WIFI";
const char* WIFI_PASSWORD = "VOTRE_MOT_DE_PASSE";
// ======================

#define DHT_PIN   4        // broche DATA du DHT22
#define DHT_TYPE  DHT22

const char* MQTT_HOST  = "192.168.20.108";
const char* MQTT_USER  = "VOTRE_UTILISATEUR";
const char* MQTT_PASS  = "VOTRE_MOT_DE_PASSE";
const int   MQTT_PORT  = 8883;                 // port securise TLS
const char* MQTT_TOPIC = "data_dht_22";
const unsigned long INTERVALLE_MS = 5000;      // un envoi toutes les 5 s

// Certificat de l'autorite de certification (CA) du broker Mosquitto
const char* CA_CERT = R"EOF(
-----BEGIN CERTIFICATE-----
...collez ici le contenu de ca.crt (certificat de votre autorite de certification)...
-----END CERTIFICATE-----
)EOF";

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClientSecure net;
PubSubClient mqtt(net);
String clientId;
unsigned long dernierEnvoi = 0;

void connecterWifi() {
  Serial.printf("Connexion Wi-Fi a %s", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWi-Fi OK, IP = %s\n", WiFi.localIP().toString().c_str());
}

void connecterMqtt() {
  while (!mqtt.connected()) {
    Serial.printf("Connexion MQTT TLS a %s:%d (client %s)... ", MQTT_HOST, MQTT_PORT, clientId.c_str());
    if (mqtt.connect(clientId.c_str(), MQTT_USER, MQTT_PASS)) {
      Serial.println("OK");
    } else {
      Serial.printf("echec, code=%d, nouvel essai dans 5 s\n", mqtt.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  dht.begin();

  // Client ID unique : CCNB-DHT22-XXXX (fin de l'adresse MAC)
  char id[24];
  snprintf(id, sizeof(id), "CCNB-DHT22-%04X", (uint16_t)(ESP.getEfuseMac() >> 32));
  clientId = id;

  connecterWifi();
  net.setCACert(CA_CERT);              // verifie le certificat du broker
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setKeepAlive(60);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connecterWifi();
  if (!mqtt.connected()) connecterMqtt();
  mqtt.loop();

  if (millis() - dernierEnvoi >= INTERVALLE_MS) {
    dernierEnvoi = millis();
    float temp = dht.readTemperature();
    float hum  = dht.readHumidity();
    if (isnan(temp) || isnan(hum)) {
      Serial.println("Lecture DHT22 impossible");
      return;
    }
    char message[64];
    snprintf(message, sizeof(message), "{\"temperature\": %.1f, \"humidity\": %.1f}", temp, hum);
    bool ok = mqtt.publish(MQTT_TOPIC, message, true);   // true = retain (le -r du tableau)
    Serial.printf("%s %s -> %s\n", ok ? "Publie" : "ECHEC", MQTT_TOPIC, message);
  }
}