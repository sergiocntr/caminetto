#include <main.h>
void setupDevices(){
  pinMode(LED_BUILTIN,OUTPUT);     //for debug
  digitalWrite(LED_BUILTIN,HIGH);

  wifiK::wifiOFF();  //spengo il wifi all'avvio
  eeK::read();
  if(ps != K_EE_OK) blinkLed(ps);
  if((settaggi.sTemp  < 0) || (settaggi.sTemp  > 100)) {

  settaggi.sMaxFan = 255;   //settaggi di default se non viene letta correttamente la eeprom 
  settaggi.sMinFan = 100;
  settaggi.sTemp = 30;
  settaggi.P = 2;
  settaggi.I = 0.5;
  settaggi.D = 0.25;
  delay(100);
  eeK::write();
  DEBUG_PRINT(msg[cs]);
}
  scPID::setupPid();  //setup PID
  sondaK::setupK();                     //configurazione scheda SPI termocoppia
  sondaDS::setupDS();
}
void setup(){
 
  Serial.begin(9600);
  delay(50);
  cs = K_ALL_OK;
  DEBUG_PRINT(msg[cs]);
  delay(500);
  
  
  DEBUG_PRINT(msg[cs]);

  setupDevices();
  delay(100);
  
  wifiK::setupWifi();  //setup Wifi
  DEBUG_PRINT(msg[cs]);
  wifiK::connWifi();
  DEBUG_PRINT(msg[cs]);
  wifiK::startMqtt();
  DEBUG_PRINT(msg[cs]);
  wifiK::sendSetUp();
  DEBUG_PRINT(msg[cs]);
  wifiK::sendStato();
  DEBUG_PRINT(msg[cs]);
}

void loop(){
  
  
  for (size_t i = 0; i < 5; i++)
   {
    sondaK::getTemperature();
    sondaDS::getTemperature();
    scPID::pidloop();

    checkStato();
    if(ps == K_REBOOT_REQ) setup();
    
    wifiK::smartDelay(6000);
   }
 
 
 if (sondaK::errNr > 0)
{
  blinkLed(1);
  delay(250);
  wifiK::sendAlarm(sondaK::errNr);
  
}
wifiK::smartDelay(10);
wifiK::sendThing();


}
void blinkLed(uint8_t volte){
  for (uint8_t i = 0; i < volte; i++)
  {
    digitalWrite(LED_BUILTIN,LOW);
    delay(250);
    digitalWrite(LED_BUILTIN,HIGH);
    delay(250);
  }
}
void checkStato(){
if(ps == K_SLEEP_REQ){ 
  wifiK::sendStato();
  wifiK::adessoDormo();
}

if(ps == K_EE_WRITE_REQ) {
  
  eeK::write();
  DEBUG_PRINT(msg[cs]);
  //wifiK::sendStato();
  eeK::read();
  DEBUG_PRINT(msg[cs]);
  wifiK::sendStato();
  wifiK::sendSetUp();

  ps = K_REBOOT_REQ;
}
}