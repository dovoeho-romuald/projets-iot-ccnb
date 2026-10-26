#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22

const char* ssid = "VOTRE_WIFI";
const char* password = "VOTRE_MOT_DE_PASSE";

IPAddress serveurCoap(192, 168, 1, 50);   // <-- IP du Raspberry Pi (hostname -I)
const int portCoap = 5683;

DHT dht(DHTPIN, DHTTYPE);
WiFiUDP udp;
Coap coap(udp);

unsigned long dernierEnvoi = 0;

// Appelée quand le serveur répond
void callback_reponse(CoapPacket &packet, IPAddress ip, int port) {
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = '\0';
  Serial.print("Réponse du serveur : ");
  Serial.println(p);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion WiFi...");
  }
  Serial.print("WiFi connecté ! IP ESP32 : ");
  Serial.println(WiFi.localIP());

  dht.begin();
  coap.response(callback_reponse);
  coap.start();
}

void loop() {
  if (millis() - dernierEnvoi >= 5000) {
    dernierEnvoi = millis();

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Échec de lecture du capteur !");
    } else {
      String payload = "Temp: " + String(t) + " Hum: " + String(h);
      Serial.println("Envoi : " + payload);
      coap.put(serveurCoap, portCoap, "capteur", payload.c_str());
    }
  }
  coap.loop();   // indispensable pour recevoir les réponses
}
