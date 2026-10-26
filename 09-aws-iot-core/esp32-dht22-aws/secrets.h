// Modèle de secrets.h : remplacez chaque valeur par la vôtre.
// Ne publiez JAMAIS votre vrai secrets.h (certificat de l'appareil + clé privée).
#include <pgmspace.h>

#define SECRET
#define THINGNAME "ESP32_DHT22"   // nom de l'objet créé dans AWS IoT Core

const char WIFI_SSID[] = "VOTRE_WIFI";
const char WIFI_PASSWORD[] = "VOTRE_MOT_DE_PASSE";
const char AWS_IOT_ENDPOINT[] = "VOTRE_ENDPOINT-ats.iot.us-east-2.amazonaws.com";  // AWS IoT > Paramètres

// Amazon Root CA 1 (fichier AmazonRootCA1.pem)
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
...collez ici le contenu de AmazonRootCA1.pem...
-----END CERTIFICATE-----
)EOF";

// Certificat de l'appareil (fichier xxxx-certificate.pem.crt)
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
...collez ici le certificat de l'appareil...
-----END CERTIFICATE-----
)KEY";

// Clé privée de l'appareil (fichier xxxx-private.pem.key)
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
...collez ici la clé privée...
-----END RSA PRIVATE KEY-----
)KEY";
