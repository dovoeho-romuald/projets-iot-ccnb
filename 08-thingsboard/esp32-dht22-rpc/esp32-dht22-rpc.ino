#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>


const char* WIFI_SSID     = "VOTRE_WIFI";
const char* WIFI_PASSWORD = "VOTRE_MOT_DE_PASSE";


const char* TB_TOKEN  = "VOTRE_CLE";
const char* TB_SERVER = "192.168.20.108";
const int   TB_PORT   = 1883;

#define DHTPIN  4
#define DHTTYPE DHT22

// --- RPC : LED integree de l'ESP32 (GPIO 2) ---
#define LED_PIN 2
bool ledState = false;

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastSend = 0;

// --- RPC : appelee quand ThingsBoard envoie un ordre ---
// Sujet recu : v1/devices/me/rpc/request/<id>
// Message    : {"method":"setState","params":true}
void onRpc(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  String sujet = String(topic);
  String id = sujet.substring(sujet.lastIndexOf('/') + 1);

  Serial.println("RPC recu : " + msg);

  String reponse;
  if (msg.indexOf("\"setState\"") >= 0 || msg.indexOf("\"setValue\"") >= 0) {
    String params = msg.substring(msg.indexOf("\"params\"") + 9);
    ledState = params.startsWith("true") || params.startsWith("1");
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    Serial.println(ledState ? "LED allumee" : "LED eteinte");
    reponse = ledState ? "true" : "false";
  } else if (msg.indexOf("\"getState\"") >= 0 || msg.indexOf("\"getValue\"") >= 0) {
    reponse = ledState ? "true" : "false";
  } else {
    reponse = "{\"error\":\"methode inconnue\"}";
  }

  // Reponse a ThingsBoard : v1/devices/me/rpc/response/<id>
  String sujetReponse = "v1/devices/me/rpc/response/" + id;
  client.publish(sujetReponse.c_str(), reponse.c_str());
}

void connectWiFi() {
  Serial.print("Connexion WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi OK, IP : ");
  Serial.println(WiFi.localIP());
}

void connectThingsBoard() {
  while (!client.connected()) {
    Serial.print("Connexion a ThingsBoard...");
    if (client.connect("ESP32-DHT22", TB_TOKEN, NULL)) {
      Serial.println(" OK");
      // --- RPC : s'abonner aux ordres de ThingsBoard ---
      client.subscribe("v1/devices/me/rpc/request/+");
    } else {
      Serial.print(" echec, code = ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  dht.begin();
  connectWiFi();
  client.setServer(TB_SERVER, TB_PORT);
  client.setCallback(onRpc);   // --- RPC
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!client.connected()) connectThingsBoard();
  client.loop();

  if (millis() - lastSend > 5000) {
    lastSend = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h)) {
      Serial.println("Erreur lecture DHT22");
      return;
    }

    String payload = "{\"temperature\":" + String(t, 1) +
                     ",\"humidity\":" + String(h, 1) + "}";

    client.publish("v1/devices/me/telemetry", payload.c_str());
    Serial.println("Envoye : " + payload);
  }
}