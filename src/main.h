#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  //#include <AsyncTCP.h>
  //#include <SPIFFS.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  //#include <ESPAsyncTCP.h>
  //	#define SPIFFS LittleFS
	//#include <LittleFS.h> 
#endif
#include <m_31855.h>
#include <m_ds18b20.h>
#include <m_Pid.h>
#include <m_Wifi.h>
#include <m_def.h>
#include <m_eeprom.h>
void blinkLed(uint8_t volte);
void checkStato();