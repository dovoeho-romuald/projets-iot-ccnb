#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// WIFI 
const char* ssid = "VOTRE_WIFI";
const char* password = "VOTRE_MOT_DE_PASSE";

WebServer server(80);

// BROCHES
#define PIN_RX 23
#define PIN_LED 4
#define PIN_BUZZER 17

#define BIT_DURATION 1000
#define START_DURATION 2000

// OLED 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ETATS
bool gazDetecte = false;
bool mouvementDetecte = false;
bool aimantDetecte = false;

unsigned long previousBlink = 0;
bool ledState = false;

// PARITE 
byte calculateParity(byte data) {
  byte count = 0;
  for (int i = 0; i < 8; i++) {
    if (bitRead(data, i)) count++;
  }
  return (count % 2 == 0) ? 1 : 0;
}

// RECEPTION NON BLOQUANTE
void receiveFrame() {

  if (digitalRead(PIN_RX) != HIGH) return;

  byte data = 0;

  delayMicroseconds(START_DURATION);

  for (int i = 0; i < 8; i++) {
    bitWrite(data, i, digitalRead(PIN_RX));
    delayMicroseconds(BIT_DURATION);
  }

  byte receivedParity = digitalRead(PIN_RX);

  if (calculateParity(data) == receivedParity) {
    gazDetecte = bitRead(data, 0);
    mouvementDetecte = bitRead(data, 1);
    aimantDetecte = bitRead(data, 2);
  }
}

// OLED 
void updateOLED() {

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("SYSTEME D'ALERTE");

  display.setCursor(0, 20);
  display.print("Gaz: ");
  display.println(gazDetecte ? "DETECTE" : "OK");

  display.setCursor(0, 30);
  display.print("Mouvement: ");
  display.println(mouvementDetecte ? "DETECTE" : "OK");

  display.setCursor(0, 40);
  display.print("Aimant: ");
  display.println(aimantDetecte ? "PROCHE" : "LOIN");

  display.display();
}

//  PAGE WEB BOOTSTRAP
void handleRoot() {

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='utf-8'>";
  html += "<meta http-equiv='refresh' content='2'>";
  html += "<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css' rel='stylesheet'>";
  html += "<title>Systeme Alerte</title>";
  html += "</head><body class='bg-dark text-white'>";

  html += "<div class='container mt-5'>";
  html += "<h1 class='text-center mb-4'>SYSTEME D'ALERTE</h1>";
  html += "<div class='row'>";

  // Gaz
  html += "<div class='col-md-4'><div class='card p-3 text-center bg-";
  html += gazDetecte ? "danger'>" : "success'>";
  html += "<h3>Gaz</h3><h4>";
  html += gazDetecte ? "DETECTE" : "OK";
  html += "</h4></div></div>";

  // Mouvement
  html += "<div class='col-md-4'><div class='card p-3 text-center bg-";
  html += mouvementDetecte ? "warning'>" : "success'>";
  html += "<h3>Mouvement</h3><h4>";
  html += mouvementDetecte ? "DETECTE" : "OK";
  html += "</h4></div></div>";

  // Aimant
  html += "<div class='col-md-4'><div class='card p-3 text-center bg-";
  html += aimantDetecte ? "danger'>" : "success'>";
  html += "<h3>Aimant</h3><h4>";
  html += aimantDetecte ? "PROCHE" : "LOIN";
  html += "</h4></div></div>";

  html += "</div></div></body></html>";

  server.send(200, "text/html", html);
}

// SETUP
void setup() {

  Serial.begin(115200);

  pinMode(PIN_RX, INPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED non detecte");
    while (true);
  }

  display.clearDisplay();
  display.display();

  // WIFI
  WiFi.begin(ssid, password);
  Serial.print("Connexion WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnecte !");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

// LOOP 
void loop() {

  receiveFrame();

  // Buzzer si gaz ou aimant
  if (gazDetecte || aimantDetecte) {
    tone(PIN_BUZZER, 1000);
  } else {
    noTone(PIN_BUZZER);
  }

  // LED clignote si mouvement
  if (mouvementDetecte) {
    if (millis() - previousBlink > 300) {
      previousBlink = millis();
      ledState = !ledState;
      digitalWrite(PIN_LED, ledState);
    }
  } else {
    digitalWrite(PIN_LED, LOW);
  }

  updateOLED();
  server.handleClient();
}
