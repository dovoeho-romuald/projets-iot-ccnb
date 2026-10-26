#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

uint8_t broadcastAddress[] = {0xEC, 0xE3, 0x34, 0xA4, 0x0C, 0x28};

typedef struct struct_message {
  int valeur;
} struct_message;

struct_message data;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {

  Serial.print("Statut envoi : ");

  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Succes");
  } else {
    Serial.println("Echec");
  }
}

void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erreur ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo = {};

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  peerInfo.ifidx = WIFI_IF_STA;   // très important

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Erreur ajout peer");
    return;
  }
}

void loop() {

  data.valeur = random(0,100);

  esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(data));

  Serial.print("Valeur envoyee : ");
  Serial.println(data.valeur);

  delay(2000);
}