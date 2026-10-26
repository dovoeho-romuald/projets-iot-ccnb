# Projets IoT / IIoT – CCNB Bathurst

Programmes, montages et configurations réalisés pendant ma formation en **Internet industriel des objets (IIoT)** au Collège communautaire du Nouveau-Brunswick (CCNB), campus de Bathurst.

Du capteur au tableau de bord : microcontrôleurs (ESP32, Arduino MKR, Nano), protocoles IoT (MQTT, MQTT/TLS, CoAP, LoRaWAN, ESP-NOW), plateformes (Node-RED, ThingsBoard, AWS IoT Core, Adafruit IO, ThingSpeak) et visualisation des données.

| Tableau de bord ThingsBoard (Raspberry Pi) | Tableau de bord AWS CloudWatch |
|---|---|
| ![ThingsBoard](docs/thingsboard-dashboard.jpg) | ![AWS CloudWatch](docs/aws-cloudwatch-dashboard.png) |

## Sommaire

| # | Dossier | Contenu | Technologies |
|---|---|---|---|
| 01 | [Bases microcontrôleurs](01-bases-microcontroleurs) | PWM, capteurs I2C/analogiques, LCD, OLED, programmation orientée objet, double cœur | Arduino, ESP32, Nano RP2040, FreeRTOS |
| 02 | [Feux de signalisation](02-feux-de-signalisation) | Feu simple puis carrefour à 4 feux en C++ orienté objet | PlatformIO, MKR WiFi 1010 |
| 03 | [Serveurs web ESP32](03-esp32-serveurs-web) | Commande de GPIO par page web, affichage de capteurs, SPIFFS, mise à jour OTA | ESP32, HTML/CSS, AsyncWebServer |
| 04 | [Communication sans fil](04-communication-sans-fil) | ESP-NOW, protocole série « maison » avec parité, LoRaWAN avec The Things Network | ESP-NOW, LoRa, TTN, MKR WAN 1310 |
| 05 | [MQTT](05-mqtt) | Interruptions + MQTT, capteurs vers broker, Adafruit IO / ThingSpeak, **MQTT sécurisé TLS** | MQTT, Mosquitto, TLS |
| 06 | [CoAP](06-coap) | ESP32 client CoAP → serveur Python sur Raspberry Pi | CoAP, aiocoap, UDP |
| 07 | [Node-RED](07-node-red) | Contrôle d'accès **RFID** et **clavier** avec vérification dans une base MariaDB | Node-RED, MariaDB, MQTT |
| 08 | [ThingsBoard](08-thingsboard) | Station DHT22 + **commande RPC** d'une LED, chaînes de règles, générateurs TBEL | ThingsBoard CE, Raspberry Pi |
| 09 | [AWS IoT Core](09-aws-iot-core) | ESP32 → AWS IoT Core (certificats X.509) → **règle IoT** → **tableau de bord CloudWatch** | AWS IoT Core, CloudWatch, IAM |

## Autres dépôts

- [Surveillance environnementale](https://github.com/dovoeho-romuald/Surveillance-environnementale) – ESP32 + serveur Node.js
- [TP Django CRUD](https://github.com/dovoeho-romuald/TP_Django_CRUD) – inventaire IoT avec Django
- [Portfolio](https://dovoeho-romuald.github.io) – tableau de bord de mes projets

## Utiliser les programmes

- Les fichiers `.ino` s'ouvrent avec l'**Arduino IDE** ; les dossiers contenant `platformio.ini` s'ouvrent avec **PlatformIO** (VS Code).
- **Aucun mot de passe n'est publié.** Avant de téléverser, remplacez les valeurs `VOTRE_WIFI`, `VOTRE_MOT_DE_PASSE`, `VOTRE_CLE`, etc. par les vôtres.
- Les adresses IP (`192.168.x.x`, `10.0.0.x`) sont celles de mon réseau local : adaptez-les à votre broker ou à votre Raspberry Pi.

Certains programmes partent d'exemples de cours ou de tutoriels (Random Nerd Tutorials, DFRobot, exemples Arduino) ; la source est indiquée en en-tête du fichier quand c'est le cas.

---
**Romuald Dovoeho** – étudiant en IIoT au CCNB · [LinkedIn](https://www.linkedin.com/in/romuald-dovoeho-aaba40327/)
