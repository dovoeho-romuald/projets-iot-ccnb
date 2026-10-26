# 05 – MQTT

| Projet | Carte | Description |
|---|---|---|
| [interruptions-porte-laser](interruptions-porte-laser) | MKR WiFi 1010 | Un capteur de porte et un faisceau laser déclenchent des **interruptions** (avec anti-rebond) ; chaque événement est publié sur un topic MQTT. |
| [dht22-mouvement-buzzer](dht22-mouvement-buzzer) | MKR WiFi 1010 | Température/humidité (DHT22) et détection de mouvement publiées sur un broker MQTT, avec alarme sonore. |
| [examen-sht40-lumiere-sol](examen-sht40-lumiere-sol) | MKR WiFi 1010 | Lumière, humidité du sol, température et humidité (SHT40) publiées sur 4 topics MQTT. |
| [adafruit-io-thingspeak](adafruit-io-thingspeak) | ESP32 | Les mêmes mesures envoyées à **deux plateformes cloud** : Adafruit IO (MQTT) et ThingSpeak (HTTP). |
| [dht22-mqtt-tls](dht22-mqtt-tls) | ESP32 | **MQTT sécurisé** : connexion chiffrée (TLS, port 8883) à mon propre broker **Mosquitto** sur Raspberry Pi, avec authentification et certificat d'autorité (CA) créé avec OpenSSL. Les données sont ensuite traitées dans Node-RED. |

**Notions travaillées :** publication/abonnement, topics, QoS, brokers (Mosquitto, test.mosquitto.org, Adafruit IO), chiffrement TLS et certificats.

## Architecture du projet MQTT/TLS

```
ESP32 + DHT22 ──MQTT/TLS (8883)──► Mosquitto (Raspberry Pi) ──► Node-RED ──► tableau de bord
```
