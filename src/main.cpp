#include <main.h>
void setup(){
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  wifiK::wifiOFF();  //spengo il wifi all'avvio
  //Serial.begin(9600);
  delay(2000);
  sondaK::offset = 2.5;     //impostazione offset termocoppia
  sondaK::setupK();         //configurazione scheda SPI termocoppia
  delay(100);
  sondaK::getTemperature();
  delay(100);
  //Serial.println("0");
  //Serial.println(lettura);
  //if(lettura < refTemp) wifiK::adessoDormo();
  //Serial.println("1");
  while (sondaK::errNr > 0)  //routine errore scheda termocoppia
  {
    wifiK::blinkLed(1);
    delay(250);
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
  
  scPID::Setpoint = 35;  //set point della ventola (il pid la farà piu veloce tanto la temperatura è piu alta )
  scPID::pid_min = 100;  //valore minimo del piedino uscita PID (default 0)
  scPID::pid_max = 255;  //valore max del piedino uscita PID (default 255)
  wifiK::smartDelay(100);
  scPID::setupPid();  //setup PID
  wifiK::smartDelay(100);
}
void loop(){
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
  if(WiFi.status() != WL_CONNECTED) wifiK::adessoDormo();
  
 // if(wifiK::t_sondaDS < refTemp) wifiK::adessoDormo();
  wifiK::sendThing();
  wifiK::smartDelay(100);

}