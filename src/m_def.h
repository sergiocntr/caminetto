#pragma once
const uint8_t versione =25;
//#define DEBUG_MIO
#ifdef DEBUG_MIO
#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define DEBUG_PRINT(str)    \
     Serial.print(__FILENAME__);     \
     Serial.print(' ');      \
     Serial.print(__LINE__);     \
     Serial.print(' ');      \
     Serial.println(str);
#else 
    #define DEBUG_PRINT(str)
#endif

struct k_setup {
 uint8_t sTemp;
 double sMinFan;
 double sMaxFan;
 double P;
 double I;
 double D;
 };k_setup settaggi;

struct k_value {
 float t_DS;
 float t_K;
 double pidVal;
 };k_value kVal;


enum camiStates {
  K_ALL_OK = 1, 
  K_PID_FAIL,         //la connessione alla scheda della termocoppia ha problemi
  K_FIRM_UPDATE,
  K_WIFI_FAIL,
  K_WIFI_OK,
  K_MQTT_FAIL
};camiStates cs;

enum eeStates { //sonos
  K_EE_OK = 0, 
  K_EE_WRITE_REQ,     //richiesta di scrivere nella eeprom
  K_EE_WRITE_FAIL,    // scrittura eeprom fallita
  K_EE_WRITE_OK, // scrittura eeprom ok
  K_EE_READ_FAIL,     // lettura eeprom fallita
  K_EE_READ_OK,  // lettura eeprom ok
  K_SLEEP_REQ,  //la connessione wifi è persa ,sleepp wifi e setup
  K_REBOOT_REQ,
};eeStates ps;

const char* msg[] = {"","K_ALL_OK","K_PID_FAIL","K_FIRM_UPDATE","K_WIFI_FAIL","K_WIFI_OK","K_MQTT_FAIL"};
const char* pmsg[] = {"K_EE_OK","K_EE_WRITE_REQ","K_EE_WRITE_FAIL","K_EE_WRITE_OK","K_EE_READ_FAIL","K_EE_READ_OK","K_SLEEP_REQ","K_REBOOT_REQ"};
unsigned long checkWifi= 300000;