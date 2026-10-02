#include <Arduino.h>

const int GPIO_AUF = 25;
const int GPIO_STOPP = 26;
const int GPIO_AB = 27;

void pressButton(int pin)
{
    Serial.print("Taste: GPIO ");
    Serial.println(pin);

    // Taste drücken
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);

    delay(300);

    // Taste loslassen
    pinMode(pin, INPUT);

    Serial.println("Taste losgelassen");
}

void setup()
{
    Serial.begin(115200);

    // Alle Tasten zunächst hochohmig
    pinMode(GPIO_AUF, INPUT);
    pinMode(GPIO_STOPP, INPUT);
    pinMode(GPIO_AB, INPUT);

    delay(2000);

    Serial.println("GW60 ESP32 gestartet");
    Serial.println("Bereit");
}

void loop()
{
    if (Serial.available())
    {
        String command = Serial.readStringUntil('\n');
        command.trim();
        command.toLowerCase();

        if (command == "up")
        {
            pressButton(GPIO_AUF);
        }
        else if (command == "stop")
        {
            pressButton(GPIO_STOPP);
        }
        else if (command == "down")
        {
            pressButton(GPIO_AB);
        }
        else
        {
            Serial.println("Unbekannter Befehl");
            Serial.println("Verwendung: up | stop | down");
        }
    }
}