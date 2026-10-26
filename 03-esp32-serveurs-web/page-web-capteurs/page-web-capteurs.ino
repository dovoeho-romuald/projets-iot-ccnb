/*
  Laboratoire 2 - CCNB IDO1 (ELTE1078)
  Affichage de donnees de capteur sur une page web - Annexe 1 (adapte pour ESP32)

  2 capteurs analogiques : LM35 (temperature) + photoresistance (lumiere ambiante)
  2 capteurs numeriques  : capteur magnetique (reed switch) + capteur tilt

  La page se rafraichit automatiquement aux 5 secondes.
*/

#include <WiFi.h>
#include <WebServer.h>

// --- Reseau WiFi ---
const char* ssid = "VOTRE_WIFI";        // <-- remplace par ton reseau (ex: celui de sketch_feb3a.ino)
const char* password = "VOTRE_MOT_DE_PASSE";   // <-- remplace par ton mot de passe

// --- Broches des capteurs ---
// Broches ADC1 (32-39) : seules celles-la restent utilisables une fois le WiFi actif
const int pinLM35 = 34;          // capteur analogique 1 : temperature (LM35)
const int pinPhotoResistance = 35; // capteur analogique 2 : lumiere ambiante

const int pinMagnetique = 4;     // capteur numerique 1 : capteur magnetique
const int pinTilt = 5;           // capteur numerique 2 : capteur tilt

WebServer server(80);

void handleRoot() {
  // --- Lecture des capteurs ---
  int lectureLM35 = analogRead(pinLM35);
  float temperature = (lectureLM35 * 3.3 / 4095.0) * 100.0; // LM35 : 10 mV/°C, ADC ESP32 sur 3.3V/12 bits

  int lumiere = analogRead(pinPhotoResistance); // 0 (sombre) a 4095 (tres lumineux)

  bool magnetiqueDetecte = (digitalRead(pinMagnetique) == LOW);  // reed switch avec INPUT_PULLUP
  bool tiltIncline = (digitalRead(pinTilt) == LOW);              // idem

  // --- Construction de la page HTML ---
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{font-family:Helvetica;text-align:center;margin-top:30px;}";
  html += ".carte{display:inline-block;background:#f0f0f0;border-radius:10px;padding:15px 30px;margin:10px;font-size:20px;}";
  html += ".ok{color:green;} .alerte{color:red;}</style></head><body>";
  html += "<h1>Tableau de bord des capteurs</h1>";

  html += "<div class='carte'>Temperature : " + String(temperature, 1) + " &deg;C</div><br>";
  html += "<div class='carte'>Niveau de lumiere ambiante : " + String(lumiere) + " / 4095</div><br>";

  html += "<div class='carte'>Capteur magnetique : <span class='" + String(magnetiqueDetecte ? "alerte" : "ok") + "'>";
  html += String(magnetiqueDetecte ? "Aimant detecte" : "Aucun aimant");
  html += "</span></div><br>";

  html += "<div class='carte'>Capteur tilt : <span class='" + String(tiltIncline ? "alerte" : "ok") + "'>";
  html += String(tiltIncline ? "Incline" : "Stable");
  html += "</span></div>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  pinMode(pinMagnetique, INPUT_PULLUP);
  pinMode(pinTilt, INPUT_PULLUP);

  Serial.print("Connexion au reseau WiFi : ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connecte.");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Serveur HTTP demarre");
}

void loop() {
  server.handleClient();
}
