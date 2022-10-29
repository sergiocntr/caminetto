#include <main.h>
void setup(){
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  wifiK::wifiOFF();  //spengo il wifi all'avvio
  Serial.begin(9600);
  cs = K_ALL_OK;
  DEBUG_PRINT(msg[cs]);
  delay(500);
  eeK::read();
  delay(50);
  DEBUG_PRINT(msg[cs]);
  if(cs == K_EP_READ_FAIL) {
  
    settaggi.sMaxFan = 255;   //settaggi di default se non viene letta correttamente la eeprom 
    settaggi.sMinFan = 100;
    settaggi.sTemp = 10;
    delay(100);
    eeK::write();
    DEBUG_PRINT(msg[cs]);
  }
  
  sondaK::setupK();                     //configurazione scheda SPI termocoppia
  delay(100);
  
  wifiK::blinkLed(cs);
  sondaK::getTemperature();
  delay(100);
  //Serial.println("0");
  //Serial.println(lettura);
  //if(lettura < refTemp) wifiK::adessoDormo();
  //Serial.println("1");
  while (sondaK::errNr > 0)  //routine errore scheda termocoppia
  {
    
    cs = K_PID_FAIL;
    wifiK::blinkLed(cs);
    delay(20);
    sondaK::getTemperature();
    delay(1250);
    sondaK::setupK();
  }
  //Serial.println("2");
  wifiK::setupWifi();  //setup Wifi
  delay(100);
  wifiK::connWifi();
  delay(100);
  DEBUG_PRINT(msg[cs]);
  delay(30);
  wifiK::startMqtt();
  DEBUG_PRINT(msg[cs]);
  wifiK::smartDelay(100);
  wifiK::sendSetUp();
  wifiK::smartDelay(100);
  DEBUG_PRINT("sendST");
  wifiK::sendStato();
}
void checkStato(){
if(cs == K_SLEEP_REQUEST){ 
  wifiK::sendStato();
  wifiK::adessoDormo();
}

if(cs == K_EE_WRITE_REQ) {
  eeK::write();
  wifiK::smartDelay(100);
  DEBUG_PRINT(msg[cs]);
  wifiK::sendStato();

  wifiK::smartDelay(100);
  
  eeK::read();
  DEBUG_PRINT(msg[cs]);
  wifiK::smartDelay(100);
  wifiK::sendStato();

  wifiK::smartDelay(100);
  wifiK::sendSetUp();

  cs = K_REBOOT_REQUEST;
}
}
void loop(){
  
  
  float lettura = 0.0;
  for (size_t i = 0; i < 5; i++)
   {
    lettura = sondaK::getTemperature();
    wifiK::t_sondaK = medianFilterK.AddValue(lettura);
    wifiK::smartDelay(100);

    lettura = sondaDS::getTemperature();
    wifiK::t_sondaDS = medianFilterDS.AddValue(lettura);
    wifiK::smartDelay(100);

    scPID::pidloop(wifiK::t_sondaDS);

    checkStato();
    if(cs == K_REBOOT_REQUEST) setup();
    
    wifiK::smartDelay(6000);
   }
 
 
 if (sondaK::errNr > 0)
{
  wifiK::blinkLed(1);
  delay(250);
  wifiK::sendAlarm(sondaK::errNr);
  
}
wifiK::smartDelay(10);
wifiK::OutPutPID = scPID::_Output;

wifiK::smartDelay(10);
wifiK::sendThing();
wifiK::smartDelay(100);

}