#pragma once
#include <EEPROM.h>
#include <m_def.h>
namespace eeK{
  void read(){
    EEPROM.begin(256);
    EEPROM.get(0, settaggi);
    ps = (EEPROM.end()) ?  K_EE_READ_OK : K_EE_READ_FAIL; 
    delay(10);
  }

  void write(){
    EEPROM.begin(256);
    EEPROM.put(0, settaggi);
    ps = EEPROM.commit() ? K_EE_WRITE_OK : K_EE_WRITE_FAIL;
    EEPROM.end();
    delay(100);
  }
}