#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "VOTRE_WIFI";
const char* password = "VOTRE_MOT_DE_PASSE";

AsyncWebServer server(80);

void setup(){

  Serial.begin(115200);

  if(!SPIFFS.begin(true)){
    Serial.println("Erreur SPIFFS");
    return;
  }

  Serial.println("SPIFFS monte correctement");

  if(!SPIFFS.exists("/index.html")){
    Serial.println("index.html introuvable !");
  }else{
    Serial.println("index.html trouve !");
  }

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connexion WiFi...");
  }

  Serial.println("Connecte au WiFi");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.begin();
}

void loop(){}