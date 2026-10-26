# 07 – Node-RED et base de données

Deux systèmes de **contrôle d'accès** : la carte envoie un identifiant par MQTT, **Node-RED** le vérifie dans une base **MariaDB/MySQL** et renvoie la réponse.

```
Carte ──MQTT──► Node-RED ──SQL──► MariaDB
  ▲                 │
  └──── réponse ────┘   (LED verte = autorisé, LED rouge = refusé)
```

## Contrôle d'accès RFID
[controle-acces-rfid](controle-acces-rfid) – **Arduino Nano 33 IoT + lecteur RFID RC522**
- Lit l'UID de la carte RFID et le publie sur `RFID/data`.
- Le flux Node-RED ([flows.json](controle-acces-rfid/flows.json)) cherche l'UID dans la base et répond `ACCESS GRANTED` ou `ACCESS DENIED` sur `RFID/data/receive`.
- LED verte : accès autorisé ; LED rouge qui clignote : carte inconnue.

## Clavier numérique + MariaDB
[clavier-mariadb](clavier-mariadb) – **Arduino MKR WiFi 1010 + clavier matriciel 4×3**
- L'utilisateur tape un numéro (10 chiffres maximum) et valide.
- Node-RED vérifie le numéro dans la base `clavierDR` et renvoie `VALIDE` ou `NON VALIDE`.
- La LED correspondante s'allume pendant 3 secondes.

## Importer le flux

Node-RED → menu ☰ → **Importer** → choisir `flows.json`. Configurez ensuite votre broker MQTT et votre base de données (les identifiants ne sont pas inclus dans le fichier).

**Notions travaillées :** Node-RED (nœuds MQTT, function, mysql), requêtes SQL, échange requête/réponse par MQTT, RFID (SPI).
