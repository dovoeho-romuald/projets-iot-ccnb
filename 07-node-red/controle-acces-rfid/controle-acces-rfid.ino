/*
 * ============================================================
 *  Système de contrôle d'accès RFID - AIIA1029
 *  Carte : Arduino Nano 33 IoT + lecteur RFID RC522
 * ============================================================
 *  Fonctionnement :
 *   1. Lit le numéro de série (UID) brut d'une carte RFID
 *   2. Envoie l'UID au broker MQTT sur le topic "RFID/data"
 *   3. Node-RED vérifie l'UID dans la base et répond sur
 *      "RFID/data/receive" : "ACCESS GRANTED" ou "ACCESS DENIED"
 *   4. LED verte = carte valide / LED rouge (clignote 2 fois) = carte inconnue
 *
 *  Code de base : newbiely.com (tutoriel RFID Nano 33 IoT),
 *  adapté pour envoyer l'UID par MQTT.
 * ============================================================
 */

#include <SPI.h>               // Bus SPI : communication avec le RC522
#include <MFRC522.h>           // Librairie du lecteur RFID RC522
#include <WiFiNINA.h>          // WiFi du Nano 33 IoT
#include <ArduinoMqttClient.h> // Client MQTT

// ---------- Broches ----------
#define SS_PIN  10 // Broche D10 reliée à SDA (SS) du RC522
#define RST_PIN 9  // Broche D9 reliée à RST du RC522
#define RED 2      // LED rouge sur D2 : carte inconnue
#define GREEN 3    // LED verte sur D3 : carte valide

// ---------- WiFi : À MODIFIER si on change de réseau ----------
char ssid[] = "VOTRE_WIFI";        // Nom du réseau WiFi (SSID)
char pass[] = "VOTRE_MOT_DE_PASSE";         // Mot de passe du WiFi

WiFiClient client;              // Connexion réseau WiFi
MqttClient mqttClient(client);  // Client MQTT qui passe par cette connexion
MFRC522 rfid(SS_PIN, RST_PIN);  // Objet qui représente le lecteur RFID

// ---------- MQTT : À MODIFIER si le serveur change ----------
const char broker[] = "192.168.20.182";            // Adresse IP du broker MQTT
int        port     = 1883;                        // Port MQTT (1883 = sans TLS)
const char topic[]  = "RFID/data";                 // Topic où l'Arduino ENVOIE l'UID
const char topicReceive[] = "RFID/data/receive";   // Topic où Node-RED RÉPOND

const long interval = 1000;        // Délai minimum entre deux envois MQTT (1 seconde)
unsigned long previousMillis = 0;  // Moment (en ms) du dernier envoi

void setup() {
  Serial.begin(9600);  // Moniteur série à 9600 bauds
  while (!Serial) {
    ; // Attend que le moniteur série soit ouvert (port USB natif)
  }

  // ---- Connexion au WiFi : réessaie toutes les 5 secondes ----
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // Échec : on affiche un point et on réessaie
    Serial.print(".");
    delay(5000);
  }

  // ---- Connexion au broker MQTT ----
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    // Échec : affiche le code d'erreur et arrête le programme
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);  // Bloque ici : il faut redémarrer l'Arduino
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // Fonction appelée automatiquement quand un message MQTT arrive
  mqttClient.onMessage(onMqttMessage);

  // ---- Abonnement au topic de réponse de Node-RED ----
  Serial.print("Subscribing to topic: ");
  Serial.println(topicReceive);
  Serial.println();

  mqttClient.subscribe(topicReceive);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topicReceive);
  Serial.println();

  // ---- Initialisation du lecteur RFID et des LED ----
  SPI.begin();         // Démarre le bus SPI
  rfid.PCD_Init();     // Initialise le RC522
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);

  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
}

void loop()
{
  mqttClient.poll();  // Vérifie si un message MQTT est arrivé (appelle onMqttMessage)

  if (rfid.PICC_IsNewCardPresent()) { // Une nouvelle carte est devant le lecteur
    if (rfid.PICC_ReadCardSerial()) { // Le numéro (UID) a bien été lu
      // Affiche le type de carte (ex. MIFARE 1KB)
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      Serial.print("RFID/NFC Tag Type: ");
      Serial.println(rfid.PICC_GetTypeName(piccType));

      // ---- Construction de l'UID en texte hexadécimal ----
      // content  = l'UID qui sera envoyé à Node-RED
      // myString = non utilisée
      // FORMAT : hexadécimal minuscule ; un octet < 0x10 est précédé
      // de " 0" (espace + zéro). Ex. octets A3 05 1F 7C -> "a3 051f7c"
      // Les UID dans la base Node-RED doivent être écrits EXACTEMENT ainsi.
      String content= "";
      String myString = "";

      Serial.print("UID:");
      for (int i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : "");     // Affichage série
        Serial.print(rfid.uid.uidByte[i], HEX);
        content += String(rfid.uid.uidByte[i]  < 0x10 ? " 0" : ""); // Ajout au texte envoyé
        content += String(rfid.uid.uidByte[i],  HEX);
      }
      Serial.println();
      Serial.println("le string: ");
      Serial.println(content);  // UID exact tel qu'envoyé par MQTT

      rfid.PICC_HaltA();      // Met la carte en veille (évite de la relire en boucle)
      rfid.PCD_StopCrypto1(); // Arrête le chiffrement du lecteur

      // ---- Envoi de l'UID par MQTT (maximum 1 fois par seconde) ----
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval)
      {
        // Mémorise le moment de cet envoi
        previousMillis = currentMillis;

        // Publie l'UID sur le topic "RFID/data"
        mqttClient.beginMessage(topic);
        mqttClient.print(content);
        mqttClient.endMessage();

        Serial.println("Data Sent");
      }
    }
  }
}

// ---- Appelée automatiquement quand Node-RED répond sur "RFID/data/receive" ----
void onMqttMessage(int messageSize) {
  // Affiche le topic et la taille du message reçu
  Serial.println("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // Lit le message caractère par caractère dans le tableau rcv
  char rcv[64];   // Tampon de 64 caractères maximum
  String myString;
  int i = 0;
  while (mqttClient.available()) {
    rcv[i++] = (char)mqttClient.read();
  }

  // Convertit le tableau de caractères en String
  for (unsigned int j = 0; j < i; j++) {
    myString += String(rcv[j]);
  }

  Serial.println(myString);  // Affiche la réponse reçue

  // ---- Allume la LED selon la réponse de Node-RED ----
  if(myString == "ACCESS GRANTED")
  {
    // Carte valide : LED verte allumée 1 seconde
    digitalWrite(GREEN, HIGH);
    delay(1000);
    digitalWrite(GREEN, LOW);
  }
  else if(myString == "ACCESS DENIED")
  {
    // Carte inconnue : LED rouge clignote 2 fois
    digitalWrite(RED, HIGH);
    delay(500);
    digitalWrite(RED, LOW);
    delay(500);
    digitalWrite(RED, HIGH);
    delay(500);
    digitalWrite(RED, LOW);
    delay(500);
  }
  Serial.println();
}
