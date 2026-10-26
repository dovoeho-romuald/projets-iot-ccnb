
#include <Wire.h>
#include <SHT40.h>
SHT40 sht40;
void setup() {
Serial.begin(9600);
while (!Serial) {
; // Wait for serial port to connect
}
Serial.println("SHT40 vs LM35");
Serial.println("===================");
// Initialize I2C
Wire.begin();
// Initialize SHT40 sensor
if (!sht40.begin(&Wire)) {
Serial.println("SHT40 initialization failed!");
Serial.println("Please check your wiring and I2C connections.");
while(1) {
delay(1000);
}
}
Serial.println("SHT40 initialized successfully");
Serial.println();
}
void loop() {
// Variables for measurements
uint16_t lm35Reading;
double lm35Temp;
// Read temperature
float sht40Temp = sht40.readTemperatureC();
lm35Reading=analogRead(A0);
lm35Temp = (double) lm35Reading * (3.3/10.24);
// Print readings on serial monitor
Serial.print("SHT40: ");
Serial.print(sht40Temp);
Serial.println(" °C");
Serial.print("LM35: ");
Serial.print(lm35Temp);
Serial.println(" °C");
Serial.println("---");
delay(2000); // Wait 2 seconds before next reading
}