#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "led_manager.h"
#include "dht_data.h"
#include "wifidata.h"

//                    1 Sekunde * 60 Sekunden * 10 Minuten
static const uint32_t DEEP_SLEEP_TIME = 1 * 1e6 * 60 * 10;
static const uint16_t MIN_VOLATGE = 3000; // below that voltage the DHT22 sensor won't work

static DhtData<4> dht;
static WiFiUDP udp;

/* https://github.com/esp8266/Arduino/blob/master/doc/reference.md#analog-input */
ADC_MODE(ADC_VCC);

static uint16_t readVoltage() {
    Serial.println("Read supply voltage");
    uint16_t voltage = ESP.getVcc();

    String outputStr = "Volatage: ";
    outputStr += voltage / 1000.0;
    outputStr += "V";
    Serial.println(outputStr);

    return voltage;
}

/**
 *
 */
static void sendData(const float &voltage, const String& dhtData)
{
    Serial.println("Connecting to server by UDP");
    if (udp.beginPacket(WifiData::SERVERNAME(), WifiData::SERVERPORT())) {
        String  outputStr("VOLTAGE=");
        outputStr += voltage;
        outputStr += ";";
        outputStr += dhtData;
        outputStr += ";TIMESTAMP=00:00:00;\n\r";

        Serial.println("Sending data by UDP to server.");
        Serial.println(outputStr);
        udp.write(outputStr.c_str());
        udp.endPacket();
        delay(200);
    }
    else {
        Serial.println("Unable to resolve server name.");
    }
}

static void deepSleep()
{
    Serial.println("Going into deep sleep for 10 minutes\n\n");
    delay(100); // time to deliver the last message
    Serial.end();
    ESP.deepSleep(DEEP_SLEEP_TIME);
}

/**
 * setup called after powerup and wakeup after deep sleep
 */
void setup() {
    LedManager.init();
    LedManager.powerOn();

    Serial.begin(115200);
    Serial.println("\nWakeup and running code");

    Serial.print("Connecting to WLAN ... ");
    WiFi.mode(WIFI_STA);
    WiFi.hostname(WifiData::HOSTNAME());
    WiFi.begin(WifiData::SSID(), WifiData::WIFI_PWD());
    if (WiFi.waitForConnectResult() == WL_CONNECTED) {
        LedManager.wlanOn();
        Serial.println("connected.");
        Serial.print("Got IP address: ");
        Serial.println(WiFi.localIP());
    }
    else {
        Serial.println("connection to WiFi failed.");
        deepSleep();
    }
}

/**
 * loop function gonna be running only once because of deep sleep at the end
 */
void loop() {
    uint16_t  voltage = readVoltage();

    if (voltage > MIN_VOLATGE) {
        Serial.println("Read data from DHT sensor");
        if (!dht.readData()) {
            Serial.println("Failed to read from DHT sensor!");
        }
    }
    else
        Serial.println("Voltage to low. Not reading sensor data. Please replace battery.");

    sendData(voltage / 1000.0, dht.dataAsAscii());

    Serial.println("Disconnecting from WiFi");
    WiFi.disconnect();
    LedManager.wlanOff();
    delay(700);

    deepSleep();
}
