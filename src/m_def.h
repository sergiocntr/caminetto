#pragma once
#include "MedianFilterLib.h"
MedianFilter<float> medianFilterK(10);
MedianFilter<float> medianFilterDS(10);
struct k_setup {
 uint8_t sTemp;
 double sMinFan;
 double sMaxFan;
 double sOffset;
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
unsigned long checkWifi= 300000;
const uint8_t versione =10;