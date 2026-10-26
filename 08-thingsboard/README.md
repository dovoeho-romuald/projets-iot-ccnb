# 08 – ThingsBoard sur Raspberry Pi

Installation de **ThingsBoard Community Edition 4.3** sur un **Raspberry Pi** (Debian, PostgreSQL, Java 17) et création d'une solution de supervision IIoT pour un campus.

![Tableau de bord ThingsBoard](../docs/thingsboard-dashboard.jpg)

## Ce qui a été réalisé

- **Modélisation du campus** : actifs hiérarchisés (campus → bâtiments → pavillons → étages → locaux), reliés par des relations *Contains*, et 5 appareils de mesure.
- **Tableau de bord** : hiérarchie d'entités, jauges, thermomètre, graphique en temps réel, interrupteur et bouton rotatif.
- **Chaînes de règles** :
  - générateurs de télémétrie en **TBEL** ([generateurs-tbel](generateurs-tbel)) ;
  - calcul de **delta** entre deux mesures ;
  - création/effacement d'**alarmes** avec envoi de **courriel** (SMTP Gmail) – [chaines-de-regles](chaines-de-regles) ;
  - débogage des nœuds et consultation des **journaux d'audit**.
- **Commande à distance (RPC)** d'un ESP32 depuis le tableau de bord.

## Station ESP32 + DHT22 avec RPC

[esp32-dht22-rpc/esp32-dht22-rpc.ino](esp32-dht22-rpc/esp32-dht22-rpc.ino)

```
ESP32 ──MQTT 1883──► ThingsBoard      télémétrie : v1/devices/me/telemetry
ESP32 ◄──────────── ThingsBoard      commandes  : v1/devices/me/rpc/request/+
ESP32 ─────────────► ThingsBoard      réponses   : v1/devices/me/rpc/response/<id>
```

- Envoie température et humidité toutes les quelques secondes.
- S'abonne aux requêtes RPC et répond à `setState` / `getState` (interrupteur) et `setValue` / `getValue` (bouton rotatif) : la **LED intégrée (GPIO 2)** s'allume ou s'éteint depuis le tableau de bord.
- Le **jeton d'accès** de l'appareil (`TB_TOKEN`) est à remplacer par le vôtre.

## Exports du Raspberry Pi

| Fichier | Contenu |
|---|---|
| [tableaux-de-bord/ccnb-laboratoire-3.json](tableaux-de-bord/ccnb-laboratoire-3.json) | Tableau de bord du campus : hiérarchie d'entités, jauges, thermomètre, graphique, interrupteur et bouton rotatif (RPC). |
| [tableaux-de-bord/station-dht22.json](tableaux-de-bord/station-dht22.json) | Tableau de bord de la station ESP32 + DHT22 : jauges de température et d'humidité, historique de la dernière heure. |
| [chaines-de-regles/flux-generation-telemetrie.json](chaines-de-regles/flux-generation-telemetrie.json) | 5 générateurs de télémétrie (locaux N376, N377, N388) et un nœud de **calcul de delta**. |
| [chaines-de-regles/root-rule-chain.json](chaines-de-regles/root-rule-chain.json) | Chaîne racine : enregistrement de la télémétrie et des attributs, appels RPC, envoi vers la chaîne d'alarme. |
| [chaines-de-regles/creer-effacer-alarme-courriel.json](chaines-de-regles/creer-effacer-alarme-courriel.json) | Création/effacement d'alarme et envoi de courriel (exemple de la documentation ThingsBoard ; scripts convertis en **TBEL**). |

## Importer ces fichiers

- **Tableau de bord** : ThingsBoard → **Tableaux de bord** → **+** → **Importer un tableau de bord** → choisir le fichier JSON. Ensuite, dans **Alias d'entités**, sélectionnez vos propres appareils.
- **Chaîne de règles** : ThingsBoard → **Chaînes de règles** → **+** → **Importer une chaîne de règles** → choisir le fichier JSON → **Appliquer**. Pour les nœuds qui pointent vers une autre chaîne ou vers un appareil, choisissez les vôtres.
