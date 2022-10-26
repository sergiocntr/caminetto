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
#include "MedianFilterLib.h"
MedianFilter<float> medianFilterK(10);
MedianFilter<float> medianFilterDS(10);
unsigned long checkWifi= 300000;
const uint8_t refTemp = 14;