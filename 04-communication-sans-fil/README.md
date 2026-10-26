# 04 – Communication sans fil

## ESP-NOW
[esp-now](esp-now) – Un ESP32 **émetteur** envoie une valeur directement à un ESP32 **récepteur** (identifié par son adresse MAC), sans routeur Wi-Fi.

## Protocole série « maison »
[protocole-serie-maison](protocole-serie-maison) – Communication entre deux ESP32 avec un **protocole conçu à la main** :
- **Émetteur A** : lit un capteur de gaz, un détecteur de mouvement et un capteur magnétique, puis envoie une trame (signal de début + 8 bits de données + **bit de parité**).
- **Récepteur B** : décode la trame sans bloquer le programme, vérifie la parité, affiche l'état sur un écran **OLED** et sur une **page web Bootstrap**, et déclenche un buzzer (gaz ou aimant) ou une LED (mouvement).

## LoRaWAN avec The Things Network (TTN)
[lora-ttn](lora-ttn) – Cartes **Arduino MKR WAN 1310** :
| Projet | Description |
|---|---|
| [led-on-off](lora-ttn/led-on-off) | Envoi de messages LoRaWAN vers TTN et réception d'une commande (downlink) pour allumer/éteindre une LED. |
| [mq7-pir-lora](lora-ttn/mq7-pir-lora) | Capteur de monoxyde de carbone **MQ-7** (moyenne glissante) + détecteur de mouvement **PIR**, données envoyées par LoRaWAN. |
| [esp32-recepteur-ttn](lora-ttn/esp32-recepteur-ttn) | Un ESP32 récupère les messages de TTN par **MQTT**, décode la charge utile (Base64/JSON) et l'affiche sur un écran LCD. |

**Notions travaillées :** adresses MAC, trames et contrôle d'erreur (parité), LoRaWAN (OTAA, uplink/downlink), intégration MQTT de TTN.

> Les clés LoRaWAN (`SECRET_APP_EUI`, `SECRET_APP_KEY`) et la clé API TTN sont remplacées par des zéros / `VOTRE_CLE_API_TTN`.
