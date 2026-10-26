# 06 – CoAP

**CoAP** (Constrained Application Protocol) est un protocole léger, basé sur UDP, pensé pour les objets à faibles ressources.

```
ESP32 + DHT22 ──CoAP PUT /capteur (UDP 5683)──► serveur Python aiocoap (Raspberry Pi)
```

| Fichier | Rôle |
|---|---|
| [esp32-client/esp32-client.ino](esp32-client/esp32-client.ino) | Lit le DHT22 toutes les 5 s et envoie la mesure au serveur avec une requête **PUT** ; affiche la réponse du serveur. |
| [serveur-raspberry-pi/serveur_coap.py](serveur-raspberry-pi/serveur_coap.py) | Serveur CoAP avec une ressource `/capteur` qui répond aux requêtes **GET**, **PUT** et **POST**. |

## Lancer le serveur

```bash
pip install aiocoap
python3 serveur_coap.py
```

Dans le sketch, remplacez l'adresse `serveurCoap` par l'IP du Raspberry Pi (`hostname -I`).

**Notions travaillées :** CoAP vs MQTT, UDP, méthodes REST (GET/PUT/POST), codes de réponse CoAP, programmation asynchrone en Python (`asyncio`). Les trames ont aussi été analysées avec Wireshark.
