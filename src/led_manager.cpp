#include <Arduino.h>
#include "led_manager.h"

void LedManagerImpl::init()
{
    pinMode(GPIO_LED_POWERUP, OUTPUT);
    pinMode(GPIO_WIFI_CONNTECT, OUTPUT);
    pinMode(GPIO_SEND_DATA, OUTPUT);
}

void LedManagerImpl::powerOn()
{
    digitalWrite(GPIO_LED_POWERUP, HIGH);
}

void LedManagerImpl::wlanOn()
{
    digitalWrite(GPIO_WIFI_CONNTECT, HIGH);
}

void LedManagerImpl::wlanOff()
{
    digitalWrite(GPIO_WIFI_CONNTECT, LOW);
}

void LedManagerImpl::toggleData()
{
    digitalWrite(GPIO_SEND_DATA, !digitalRead(GPIO_SEND_DATA));
}

// singleton of LedManager
LedManagerImpl   LedManager;
