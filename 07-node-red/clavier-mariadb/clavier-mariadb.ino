#include <WiFiNINA.h>         // Pour connecter la MKR WiFi 1010 au Wi-Fi
#include <Keypad.h>           // Pour utiliser le clavier matriciel
#include <ArduinoMqttClient.h> // Pour communiquer avec Node-RED par MQTT
//#include <WiFi101.h>          // Pour connecter le MKR1000 au Wi-Fi

// 1. CONFIGURATION DU WIFI

// Nom et mot de passe du reseau Wi-Fi utilise
char ssid[] = "VOTRE_WIFI";
char pass[] = "VOTRE_MOT_DE_PASSE";

// 2. CONFIGURATION MQTT

// On cree les objets necessaires pour la communication MQTT
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

// Adresse IP du Raspberry Pi et port du broker MQTT
const char broker[] = "192.168.20.182";
int port = 1883;

// Topic pour envoyer le numero saisi
const char topic[] = "romuald/data";

// Topic pour recevoir la reponse de Node-RED
const char topicReponse[] = "romuald/reponse";

// 3. CONFIGURATION DES LED

// La LED verte indique que le numero est autorise
const int ledVerte = 6;

// La LED rouge indique que le numero est refuse
const int ledRouge = 7;

// Variables utilisees pour eteindre les LED apres 3 secondes
unsigned long debutAffichage = 0;
bool resultatAffiche = false;

// Variables pour savoir si on attend une reponse de Node-RED
bool attenteReponse = false;
unsigned long debutAttente = 0;

// 4. CONFIGURATION DU CLAVIER MATRICIEL

// Notre clavier possede 4 lignes et 3 colonnes
const byte ROWS = 4;
const byte COLS = 3;


// On indique la position des touches du clavier
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

// Broches utilisees pour brancher le clavier sur le MKR1000
byte rowPins[ROWS] = {0, 1, 2, 3};
byte colPins[COLS] = {4, 5, 8};

// On cree notre clavier avec les touches et les broches definies
Keypad customKeypad = Keypad(
  makeKeymap(hexaKeys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

// Cette variable garde les chiffres taper par l'utilisateur
String numeroAcces = "";

// On limite la saisie a 10 chiffres maximum
const int longueurMax = 10;

// 5. FONCTION DE CONNEXION WIFI
void connecterWiFi() {
  Serial.println("Connexion WiFi...");

  // On essaie de se connecter tant que le Wi-Fi n'est pas disponible
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(2000);
  }

  // Ce message confirme que la connexion a reussi
  Serial.println("WiFi connecte !");
}

// 6. FONCTION DE CONNEXION MQTT

void connecterMQTT() {
  Serial.println("Connexion MQTT...");

  // On essaie de joindre le broker MQTT du Raspberry Pi
  while (!mqttClient.connect(broker, port)) {
    delay(2000);
  }
 Serial.println("MQTT connecte !");

  // On s'abonne au topic pour recevoir les resultats de Node-RED
  mqttClient.subscribe(topicReponse);
}

// 7. RECEPTION ET TRAITEMENT DES REPONSES

void messageRecu(int messageSize) {

  // Variable pour enregistrer la reponse reçue
  String reponse = "";

  // On lit le message envoye par Node-RED
  while (mqttClient.available()) {
    reponse += (char)mqttClient.read();
  }

  // On enleve les espaces inutiles
  reponse.trim();

  // On affiche la reponse dans le moniteur serie
  Serial.print("Reponse : ");
  Serial.println(reponse);

  // Si aucun numero n'est en attente, on ignore le message
  if (!attenteReponse) {
    return;
  }

  // On eteint d'abord les deux LED
  digitalWrite(ledVerte, LOW);
  digitalWrite(ledRouge, LOW);

  // Si le numero existe dans MariaDB, Node-RED renvoie VALIDE
  if (reponse == "VALIDE") {

    // On allume la LED verte pour autoriser l'acces
    digitalWrite(ledVerte, HIGH);
    Serial.println("ACCES AUTORISE");

  }

  // Si le numero n'existe pas, Node-RED renvoie NON VALIDE
  else if (reponse == "NON VALIDE") {

    // On allume la LED rouge pour refuser l'acces
    digitalWrite(ledRouge, HIGH);
    Serial.println("ACCES REFUSE");
 }
  // Si la reponse est differente, on ne fait rien
  else {
    return;
  }

  // On memorise le moment ou la LED s'est allumee
  debutAffichage = millis();
  resultatAffiche = true;

  // La verification est terminee
  attenteReponse = false;
}

void setup() {
  // On demarre le moniteur serie
  Serial.begin(115200);

  // On configure les broches des LED en sortie
  pinMode(ledVerte, OUTPUT);
  pinMode(ledRouge, OUTPUT);

  // Au demarrage, les deux LED restent eteintes
  digitalWrite(ledVerte, LOW);
  digitalWrite(ledRouge, LOW);

  // On indique la fonction qui traitera les reponses MQTT
  mqttClient.onMessage(messageRecu);

  // On connecte le MKR1000 au Wi-Fi puis au broker MQTT
  connecterWiFi();
  connecterMQTT();

  // Le systeme est maintenant pret a recevoir un numero
  Serial.println("Systeme pret !");
}
void loop() {

  // On verifie si le MKR1000 est toujours connecte au Wi-Fi
  if (WiFi.status() != WL_CONNECTED) {

    // On annule la verification en cours
    attenteReponse = false;
    resultatAffiche = false;

    // On eteint les LED
    digitalWrite(ledVerte, LOW);
    digitalWrite(ledRouge, LOW);

    // On essaie de reconnecter le Wi-Fi
    connecterWiFi();
  }

  // On verifie si la connexion MQTT fonctionne toujours
  if (!mqttClient.connected()) {

   
 // On annule la demande en cours
    attenteReponse = false;
    resultatAffiche = false;

    // On eteint les LED
    digitalWrite(ledVerte, LOW);
    digitalWrite(ledRouge, LOW);

    // On reconnecte le MKR1000 au broker MQTT
    connecterMQTT();
  }

  // On maintient la communication MQTT et on lit les messages recus
  mqttClient.poll();

  // On verifie si une touche du clavier a ete appuyee
  char touche = customKeypad.getKey();
  if (touche) {

    // Si l'utilisateur appuie sur un chiffre de 0 a 9
    if (touche >= '0' && touche <= '9') {

      // On ajoute le chiffre si la limite de 10 n'est pas atteinte
      if (numeroAcces.length() < longueurMax) {
        numeroAcces += touche;

        // On affiche le numero en cours de saisie
        Serial.print("Numero : ");
        Serial.println(numeroAcces);
      }
    }

    // Si l'utilisateur appuie sur *, on efface le numero
    else if (touche == '*') {

      numeroAcces = "";
      Serial.println("Numero efface");
    }

    // Si l'utilisateur appuie sur #, on valide la saisie
    else if (touche == '#') {

      // On envoie seulement si un numero a ete saisi
      // et qu'aucune autre verification n'est en cours
      if (numeroAcces.length() > 0 && !attenteReponse) {

        Serial.print("Envoi : ");
        Serial.println(numeroAcces);

        // On eteint les LED avant de faire une nouvelle verification
        digitalWrite(ledVerte, LOW);
        digitalWrite(ledRouge, LOW);
        resultatAffiche = false;

        // On prepare l'envoi du numero sur le topic Keypad/data
        if (mqttClient.beginMessage(topic)) {

          // On place le numero dans le message MQTT
          mqttClient.print(numeroAcces);

          // On termine l'envoi du message
          if (mqttClient.endMessage()) {

            Serial.println("Numero envoye !");

            // On attend maintenant la reponse de Node-RED
            attenteReponse = true;

            // On memorise le moment de l'envoi
            debutAttente = millis();

          } else {

            // Message affiche si l'envoi echoue
            Serial.println("Erreur d'envoi MQTT");
          }
        }

        // On vide la saisie pour le prochain numero
        numeroAcces = "";
      }
    }
  }

  // On eteint la LED apres 3 secondes
  if (resultatAffiche &&
      millis() - debutAffichage >= 3000) {
    digitalWrite(ledVerte, LOW);
    digitalWrite(ledRouge, LOW);
    resultatAffiche = false;
  }

  // Si Node-RED ne repond pas apres 5 secondes on annule la demande
  if (attenteReponse &&
      millis() - debutAttente >= 5000) {
    attenteReponse = false;
    Serial.println("Delai de reponse depasse");
  }
}
