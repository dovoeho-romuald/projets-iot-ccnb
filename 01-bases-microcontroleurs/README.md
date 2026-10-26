# 01 – Bases des microcontrôleurs

| Projet | Carte | Ce qu'il fait |
|---|---|---|
| [moteur-dc-pwm](moteur-dc-pwm) | Arduino | Fait accélérer puis ralentir un moteur DC avec la modulation de largeur d'impulsion (PWM). |
| [sht40-vs-lm35](sht40-vs-lm35) | Arduino | Compare deux capteurs de température : SHT40 (numérique, I2C) et LM35 (analogique). |
| [mario-lcd-capteur-magnetique](mario-lcd-capteur-magnetique) | Arduino | Quand un aimant est détecté, l'écran LCD RGB change de couleur et un buzzer joue la musique de Super Mario. |
| [capteur-actionneur-poo](capteur-actionneur-poo) | Nano RP2040 Connect | Classes C++ `Led` et `Lumiere` : la LED clignote quand la lumière ambiante est faible. |
| [esp32-double-coeur](esp32-double-coeur) | ESP32 | Deux tâches FreeRTOS exécutées en parallèle sur les deux cœurs de l'ESP32. |
| [ecran-oled-i2c](ecran-oled-i2c) | ESP32 | Affichage de texte sur un écran OLED SSD1306 (I2C). |

**Notions travaillées :** entrées/sorties numériques et analogiques, PWM, bus I2C, programmation orientée objet en C++, multitâche (FreeRTOS).
