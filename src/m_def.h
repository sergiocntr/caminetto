#pragma once
const uint8_t versione =12;
#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define DEBUG_PRINT(str)    \
     Serial.print(__FILENAME__);     \
     Serial.print(' ');      \
     Serial.print(__LINE__);     \
     Serial.print(' ');      \
     Serial.println(str);
#include "MedianFilterLib.h"
MedianFilter<float> medianFilterK(10);
MedianFilter<float> medianFilterDS(10);
struct k_setup {
 uint8_t sTemp;
 double sMinFan;
 double sMaxFan;
 
};
k_setup settaggi;
enum camiStates {
  K_ALL_OK = 1, 
  K_EP_READ_FAIL,     // lettura eeprom fallita
  K_EP_READ_SUCCESS,  // lettura eeprom ok
  K_PID_FAIL,         //la connessione alla scheda della termocoppia ha problemi
  K_EE_WRITE_REQ,     //richiesta di scrivere nella eeprom
  K_EP_WRITE_FAIL,    // scrittura eeprom fallita
  K_EP_WRITE_SUCCESS, // scrittura eeprom ok
  K_SLEEP_REQUEST,  //la connessione wifi è persa ,sleepp wifi e setup
  K_REBOOT_REQUEST,
  K_FIRM_UPDATE,
  K_WIFI_FAIL,
  K_WIFI_OK,
  K_MQTT_FAIL
  
  
};
camiStates cs;
const char* msg[] = {"","K_ALL_OK","K_EP_READ_FAIL","K_EP_READ_SUCCESS","K_PID_FAIL","K_EE_WRITE_REQ","K_EP_WRITE_FAIL","K_EP_WRITE_SUCCESS","K_SLEEP_REQUEST","K_REBOOT_REQUEST","K_FIRM_UPDATE","K_WIFI_FAIL", "K_WIFI_OK","K_MQTT_FAIL"};

unsigned long checkWifi= 300000;