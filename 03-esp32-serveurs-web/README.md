# 03 – Serveurs web sur ESP32

L'ESP32 se connecte au Wi-Fi et héberge lui-même une page web, accessible depuis un navigateur du même réseau.

| Projet | Description |
|---|---|
| [commande-gpio-web](commande-gpio-web) | Page web avec boutons ON/OFF pour allumer et éteindre deux sorties (LED, relais). |
| [page-web-capteurs](page-web-capteurs) | Affiche en direct 4 capteurs : LM35 (température), photorésistance (lumière), capteur magnétique et capteur d'inclinaison. La page se rafraîchit toutes les 5 secondes. |
| [serveur-async-spiffs](serveur-async-spiffs) | Serveur web asynchrone : la page `index.html` est stockée dans la mémoire flash (SPIFFS) de l'ESP32. |
| [mise-a-jour-ota](mise-a-jour-ota) | Mise à jour du programme **sans fil** (OTA) : on envoie le nouveau firmware depuis une page web protégée par identifiant. |

**Notions travaillées :** HTTP, HTML/CSS, système de fichiers SPIFFS, mise à jour OTA, mDNS.

> Remarque : les broches ADC2 de l'ESP32 ne fonctionnent pas quand le Wi-Fi est actif ; les capteurs analogiques sont donc branchés sur l'ADC1 (GPIO 32 à 39).
