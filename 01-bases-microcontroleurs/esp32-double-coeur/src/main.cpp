#include <Arduino.h>   // ➡ AJOUTÉ (nécessaire avec VS Code / PlatformIO)

// Déclaration des tâches
TaskHandle_t Task1;
TaskHandle_t Task2;

// LED sur GPIO 2
const int ledPin = 2;

// ==========================
// Tâche sur Core 0
// ==========================
void TaskBlink(void *pvParameters) {

  pinMode(ledPin, OUTPUT);   // ➡ MODIFIÉ (j'ai enlevé le "." qui causait une erreur)

  while (true) {

    digitalWrite(ledPin, HIGH);

    Serial.print("LED sur Core : ");      // ➡ AJOUTÉ
    Serial.println(xPortGetCoreID());     // ➡ AJOUTÉ (affiche le core utilisé)

    vTaskDelay(500 / portTICK_PERIOD_MS);

    digitalWrite(ledPin, LOW);

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// ==========================
// Tâche sur Core 1
// ==========================
void TaskSerial(void *pvParameters) {

  while (true) {

    Serial.print("Message serie sur Core : ");   // ➡ MODIFIÉ
    Serial.println(xPortGetCoreID());            // ➡ AJOUTÉ

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {

  Serial.begin(115200);
  delay(1000);     // ➡ AJOUTÉ (laisser le temps au moniteur série)

  // Création de la tâche sur Core 0
  xTaskCreatePinnedToCore(
    TaskBlink,
    "TaskBlink",
    1000,
    NULL,
    1,
    &Task1,
    0);     // Core 0

  delay(500);

  // Création de la tâche sur Core 1
  xTaskCreatePinnedToCore(
    TaskSerial,
    "TaskSerial",
    1000,
    NULL,
    1,
    &Task2,
    1);     // Core 1
}

void loop() {
}