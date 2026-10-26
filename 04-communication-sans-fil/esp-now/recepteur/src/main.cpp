#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

typedef struct struct_message {
  int valeur;
} struct_message;

struct_message data;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  memcpy(&data, incomingData, sizeof(data));

  Serial.print("Valeur recue : ");
  Serial.println(data.valeur);
}

void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erreur ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}