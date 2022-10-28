#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <m_def.h>
namespace eeK{
  void read(){
    EEPROM.begin(256);
    delay(100);
    EEPROM.get(0, settaggi);
    cs = (EEPROM.end()) ?  K_EP_READ_SUCCESS : K_EP_READ_FAIL; 
  }

  void write(){
    EEPROM.begin(256);
    delay(100);
    EEPROM.put(0, settaggi);
    cs = EEPROM.commit() ? K_EP_WRITE_SUCCESS : K_EP_WRITE_FAIL;
  }
}