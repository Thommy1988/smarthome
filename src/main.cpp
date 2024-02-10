#include <Arduino.h>

// Definitionen für die LED und serielle Kommunikation
const int ledPin = LED_BUILTIN;  // Die meisten ESP8266-Boards haben eine eingebaute LED am Pin D4 (GPIO2)

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);  // Starten der seriellen Kommunikation mit einer Baudrate von 115200
  Serial.println("Hello World!");  // Ausgabe der Nachricht beim Start
}

void loop() {
  digitalWrite(ledPin, HIGH);   // LED einschalten
  delay(1000);                   // 1 Sekunde warten
  digitalWrite(ledPin, LOW);    // LED ausschalten
  delay(1000);                   // 1 Sekunde warten

  // Nachricht über die serielle Konsole ausgeben
  Serial.println("Hello World from Loop!");
}
