#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "secrets.h"

// =========================
// GW60 GPIOs
// =========================

const int GPIO_UP   = 25;
const int GPIO_STOP = 26;
const int GPIO_DOWN = 27;

// =========================
// WLAN / MQTT
// =========================

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// =========================
// GW60 Taste drücken
// =========================

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

// =========================
// MQTT Befehle
// =========================

void mqttCallback(char* topic, byte* payload, unsigned int length)
{
    String command;

    for (unsigned int i = 0; i < length; i++)
    {
        command += (char)payload[i];
    }

    command.trim();
    command.toLowerCase();

    Serial.print("MQTT Befehl: ");
    Serial.println(command);

    if (command == "up")
    {
        pressButton(GPIO_UP);
    }
    else if (command == "stop")
    {
        pressButton(GPIO_STOP);
    }
    else if (command == "down")
    {
        pressButton(GPIO_DOWN);
    }
    else
    {
        Serial.println("Unbekannter MQTT Befehl");
    }
}

// =========================
// WLAN verbinden
// =========================

void connectWiFi()
{
    Serial.print("Verbinde mit WLAN: ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WLAN verbunden");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.localIP());
}

// =========================
// MQTT verbinden
// =========================

void connectMQTT()
{
    while (!mqttClient.connected())
    {
        Serial.print("Verbinde mit MQTT... ");

        if (mqttClient.connect("esp32-gw60", MQTT_USER, MQTT_PASSWORD))
        {
            Serial.println("verbunden");

            mqttClient.subscribe("smarthome/gw60/command");

            Serial.println("MQTT Topic abonniert:");
            Serial.println("smarthome/gw60/command");
        }
        else
        {
            Serial.print("Fehler, State=");
            Serial.println(mqttClient.state());

            delay(5000);
        }
    }
}

// =========================
// Setup
// =========================

void setup()
{
    Serial.begin(115200);

    // Alle Tasten zunächst hochohmig
    pinMode(GPIO_UP, INPUT);
    pinMode(GPIO_STOP, INPUT);
    pinMode(GPIO_DOWN, INPUT);

    delay(500);

    Serial.println();
    Serial.println("==========================");
    Serial.println("GW60 ESP32 gestartet");
    Serial.println("==========================");

    connectWiFi();

    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setCallback(mqttCallback);

    connectMQTT();

    Serial.println("System bereit");
}

// =========================
// Loop
// =========================

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        connectWiFi();
    }

    if (!mqttClient.connected())
    {
        connectMQTT();
    }

    mqttClient.loop();
}