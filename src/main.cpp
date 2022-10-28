#include <main.h>
void setup(){
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  wifiK::wifiOFF();  //spengo il wifi all'avvio
  //Serial.begin(9600);
  cs = K_ALL_OK;
  wifiK::blinkLed(cs);
  delay(2000);
  if(cs == K_EP_READ_FAIL) {
  
    settaggi.sMaxFan = 255;   //settaggi di default se non viene letta correttamente la eeprom 
    settaggi.sMinFan = 100;
    settaggi.sTemp = 10;
    settaggi.sOffset = 1.2;
    delay(100);
    eeK::write();
   delay(100);
  }
  wifiK::blinkLed(cs);
  delay(2000);
  sondaK::offset = settaggi.sOffset;     //impostazione offset termocoppia
  sondaK::setupK();                     //configurazione scheda SPI termocoppia
  delay(100);
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
  //Serial.println("3");
  delay(100);
  wifiK::startMqtt();
  //Serial.println("4");
 
  sondaDS::setupDS();  //setup sonda DS18b20
  wifiK::smartDelay(100);
  
  scPID::Setpoint = settaggi.sTemp;  //set point della ventola (il pid la farà piu veloce tanto la temperatura è piu alta )

  wifiK::smartDelay(100);
  scPID::setupPid();  //setup PID
  wifiK::smartDelay(100);
  wifiK::sendSetUp();
  wifiK::smartDelay(100);
  wifiK::sendStato();
}
void loop(){
  if(cs == K_REBOOT_REQUEST) setup();
  float lettura = 0.0;
  //float medianK = 0.0;
  //float medianDS = 0.0;
   for (size_t i = 0; i < 5; i++)
   {
    lettura = sondaK::getTemperature();
    delay(100);
    wifiK::t_sondaK = medianFilterK.AddValue(lettura);
    wifiK::smartDelay(100);
 //   Serial.print("K is ");
	//	Serial.println(lettura);
 //   Serial.print("K median is ");
//		Serial.println(wifiK::t_sondaK);
    //delay(100);
    lettura = sondaDS::getTemperature();
    delay(100);
    wifiK::t_sondaDS = medianFilterDS.AddValue(lettura);
    scPID::pidloop(wifiK::t_sondaDS);
    //wifiK::smartDelay(100);
   // Serial.print("DS is ");
	//	Serial.println(lettura);
   // Serial.print("DS median is ");
	//	Serial.println(wifiK::t_sondaDS);
    wifiK::smartDelay(6000);
   }
 
 if (sondaK::errNr > 0)
  {
    wifiK::blinkLed(1);
    delay(250);
    wifiK::sendAlarm(sondaK::errNr);
    
  }


delay(10);
wifiK::OutPutPID = scPID::_Output;
  //if(avg< 35) return;
 // wifiK::t_sondaK =medianK; 
    // Serial.print("K media ");
		// Serial.println(wifiK::t_sondaK);
  //wifiK::t_sondaDS  =medianDS ;
     // Serial.print("DS media ");
	//	Serial.println(wifiK::t_sondaDS);
  delay(10);
  wifiK::sendThing();
  wifiK::smartDelay(100);
  if(cs == K_SLEEP_REQUEST){ 
    wifiK::sendStato();
    wifiK::adessoDormo();
  }
  if(cs == K_EE_WRITE_REQ) {
    eeK::write();
    delay(100);
    wifiK::sendStato();
    delay(100);
    eeK::read();
    delay(100);
    wifiK::sendStato();
    delay(100);
  }

}