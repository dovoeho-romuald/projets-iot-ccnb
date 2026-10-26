#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// 🔹 Mets ton Wi-Fi ici
const char* ssid = "VOTRE_WIFI";
const char* password = "VOTRE_MOT_DE_PASSE";

// GPIO utilisés
const int output26 = 26;
const int output27 = 27;

String output26State = "off";
String output27State = "off";

// Serveur web port 80
WebServer server(80);

// ===== Fonctions GPIO =====
void handleRoot();

void handleGPIO26On() {
  output26State = "on";
  digitalWrite(output26, HIGH);
  handleRoot();
}

void handleGPIO26Off() {
  output26State = "off";
  digitalWrite(output26, LOW);
  handleRoot();
}

void handleGPIO27On() {
  output27State = "on";
  digitalWrite(output27, HIGH);
  handleRoot();
}

void handleGPIO27Off() {
  output27State = "off";
  digitalWrite(output27, LOW);
  handleRoot();
}

// ===== Page Web =====
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body{text-align:center;font-family:Arial;}";
  html += ".button{padding:15px 30px;font-size:20px;margin:5px;}";
  html += "</style></head><body>";

  html += "<h1>ESP32 Web Server</h1>";

  html += "<p>GPIO 26 : " + output26State + "</p>";
  if(output26State=="off")
    html += "<a href='/26/on'><button class='button'>ON</button></a>";
  else
    html += "<a href='/26/off'><button class='button'>OFF</button></a>";

  html += "<p>GPIO 27 : " + output27State + "</p>";
  if(output27State=="off")
    html += "<a href='/27/on'><button class='button'>ON</button></a>";
  else
    html += "<a href='/27/off'><button class='button'>OFF</button></a>";

  html += "</body></html>";

  server.send(200,"text/html",html);
}

// ===== Setup =====
void setup() {
  Serial.begin(115200);

  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);

  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

  // Connexion Wi-Fi
  Serial.print("Connexion WiFi...");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnecté !");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Routes
  server.on("/",handleRoot);
  server.on("/26/on",handleGPIO26On);
  server.on("/26/off",handleGPIO26Off);
  server.on("/27/on",handleGPIO27On);
  server.on("/27/off",handleGPIO27Off);

  server.begin();
}

// ===== Loop =====
void loop(){
  server.handleClient();
}
