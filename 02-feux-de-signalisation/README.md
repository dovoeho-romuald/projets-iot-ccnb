# 02 – Feux de signalisation

Deux versions d'un système de feux routiers, programmées en **C++ orienté objet** avec **PlatformIO**.

| Projet | Description |
|---|---|
| [feu-avec-timer](feu-avec-timer) | Un feu (vert → jaune → rouge) géré par une classe `Feu` et une minuterie non bloquante (`millis()`). |
| [carrefour-4-feux](carrefour-4-feux) | Un carrefour complet : 4 objets `Feu` coordonnés par une classe `Carrefour`, pour que deux directions ne soient jamais vertes en même temps. |

**Notions travaillées :** classes et encapsulation, fichiers d'en-tête (`.h`), machine à états, temporisation sans `delay()`.
