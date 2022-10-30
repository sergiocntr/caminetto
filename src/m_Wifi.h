#pragma once
#include <Arduino.h>
#ifdef ESP8266
  #include <ESP8266WiFi.h> 
  #include <ESP8266HTTPClient.h>
  #include <ESP8266httpUpdate.h>
#endif //ESP8266
// QUI VANNO LIBRERIE SPECIFICHE PER ESP 32
#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
  #include <HTTPUpdate.h>
#endif  //ESP32
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <topic.h>
#include <myIP.h>
#include "password.h"
#include <m_def.h>
namespace wifiK{
WiFiClient mywifi;
WiFiClient c;
PubSubClient client(c);
    
bool mqttOK=0;
const uint32_t wifi_check_time=10000L;
uint32_t wifi_initiate =0;
//QUESTO CODICE NON BLOCCANTE LO UTILIZZO AL POSTO DEL DELAY
void smartDelay(uint32_t ms){
  uint32_t start = millis();
  do
  {
    client.loop();
    delay(10);
  } while (millis() - start < ms);
}

//ROUTINE PER AGGIORNAMENTO FIRM VIA WEB
uint8_t checkForUpdates(uint8_t FW_VERSION) {
  uint8_t check=0;
  String fwURL = String( fwUrlBase );
  fwURL.concat( CamiId ); //QUI DA VEDERE CON PERCORSO SERVER
  String fwVersionURL = fwURL;
  fwVersionURL.concat( "/version.php" );
  String fwImageURL = fwURL;
  fwImageURL.concat( "/firmware.bin" );
  delay(100);
  WiFiClient myUpdateConn;
  HTTPClient http;
  http.begin( myUpdateConn, fwVersionURL );
  int httpCode = http.GET();
  if( httpCode == 200 ) {
    String newFWVersion = http.getString();
    int newVersion = newFWVersion.toInt();
    if( newVersion > FW_VERSION ) {
    //  //Serial.println( "Preparing to update" );
      t_httpUpdate_return ret = ESPhttpUpdate.update(myUpdateConn , fwImageURL );
      switch(ret) {
        case HTTP_UPDATE_FAILED:
          check=1;
          //Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
          break;

        case HTTP_UPDATE_NO_UPDATES:
          check=2;
          //Serial.println("HTTP_UPDATE_NO_UPDATES");
          break;
        case HTTP_UPDATE_OK:
          //Serial.println("[update] Update ok."); // may not called we reboot the ESP
          break;
      }
    }
    else {
      check=0;
      //Serial.println( "Already on latest version" );
    }
  }
  else {
    //Serial.print( "Firmware version check failed, got HTTP response code " );
    //Serial.println( httpCode );
    check= httpCode;
  }
  http.end();
  myUpdateConn.stop();
  return check;
}

//CALLBACK QUANDO IL MODULO RICEVE VIA MQTT QUALCHE MESSAGGIO =IN QUESTO CASO SOLO LA RICHIESTA DI PROCEDERE ALL'AGGIORNAMENTO FIRM)
void callback(char* topic, byte* payload, unsigned int length) {
  if(strcmp(topic,systemTopic) == 0){
    if (char(payload[0]) == '0') {
      delay(10);
      
      ps = K_SLEEP_REQ;
    }
  }
  if(strcmp(topic,updateTopic) == 0){
    delay(10);
    byte mioDato = payload[0];
    if(mioDato==52){ //
      client.publish(logCamiTopic, "agg. Caminetto");
      delay(10);
      uint8_t miocheck = 0;
      miocheck = checkForUpdates(versione);
      switch(miocheck) {
        case 1:
          client.publish(logCamiTopic, "HTTP_UPDATE_FAIL"); 
          break;
        case 2:
          client.publish(logCamiTopic, "HTTP_UPDATE_NO_UPDATES");
          break;
        case 0:
          client.publish(logCamiTopic, "Already on latest version" );
          break;
        default:
          //client.publish(logCamiTopic, "Firmware version check failed, got HTTP response code " + String(miocheck));
          break;
      }
    }
  }
  
  else if(strcmp(topic, setupCamiTopic) == 0 ) {
    StaticJsonDocument<256> doc;
    DeserializationError er = deserializeJson(doc, payload);
    smartDelay(10);
    if(er) return;
    const char* to = doc["topic"];
    smartDelay(10);
    DEBUG_PRINT(to);
    if(strcmp(to,"AggCami") != 0) return;
    
    //client.publish(logCamiTopic, "agg val");
    double camiVal = doc["sT"];
    smartDelay(10);
    if(camiVal > 0)  {settaggi.sTemp = camiVal;}
    smartDelay(10);
    camiVal = doc["mF"];
    if(camiVal > 0)  {settaggi.sMinFan = camiVal;}
    smartDelay(10);
    camiVal = doc["MF"];
    if(camiVal > 0)  {settaggi.sMaxFan = camiVal;}
    camiVal = doc["P"];
    smartDelay(10);
    if(camiVal > 0)  {settaggi.P = camiVal;}
    smartDelay(10);
    camiVal = doc["I"];
    if(camiVal > 0)  {settaggi.I = camiVal;}
    smartDelay(10);
    camiVal = doc["D"];
    if(camiVal > 0)  {settaggi.D = camiVal;}
    
    ps = K_EE_WRITE_REQ; //flag
  }
  
}

//  ROUTINE PER CREARE JSON DA MANDARE VIA MQTT
void sendThing() {
  StaticJsonDocument<256> doc;
  delay(10);
  doc["topic"] = "Cami";
  delay(10);
  doc["PID"] = kVal.pidVal;
  delay(10);
  doc["K"] = kVal.t_K;
  delay(10);
  doc["DS"] = kVal.t_DS;
  delay(10);
  doc["stato"] = msg[cs];
  char buffer[256];
  delay(10);
  size_t n = serializeJson(doc, buffer);
  delay(10);
  mqttOK = client.publish(casaSensTopic, buffer,n);
  //delay(10);
  //Serial.println(mqttOK);
  smartDelay(1000);
  //serializeJsonPretty(doc, Serial);
 if(mqttOK)  {cs = K_ALL_OK;} else {ps = K_SLEEP_REQ;}
}
void sendSetUp() {
  StaticJsonDocument<512> doc;
  delay(10);
  doc["topic"] = "SCami";
  delay(10);
  doc["mF"] = settaggi.sMinFan;
  delay(10);
  doc["MF"] = settaggi.sMaxFan;
  delay(10);
  doc["sT"] = settaggi.sTemp;
  delay(10);
  doc["P"] = settaggi.P;
  delay(10);
  doc["I"] = settaggi.I;
  delay(10);
  doc["D"] = settaggi.D;
  char buffer[512];
  delay(10);
  size_t n = serializeJson(doc, buffer);
  delay(10);
  mqttOK = client.publish(setupCamiTopic, buffer,n);
  //delay(10);
  //Serial.println(mqttOK);
  smartDelay(1000);
  //serializeJsonPretty(doc, Serial);
 // if(mqttOK)  cs = K_ALL_OK; else ps = K_SLEEP_REQ;
}
void sendStato() {
 
    mqttOK = client.publish(logCamiTopic,msg[cs]);
    smartDelay(1000);
    mqttOK = client.publish(logCamiTopic,pmsg[ps]);
    smartDelay(1000);
 //   if(mqttOK)  cs = K_ALL_OK; else ps = K_SLEEP_REQ;
 
}


//QUESTO SOTTOSCRIVE MQTT AI TOPIC 
void reconnect() {
  smartDelay(50);
  String s = "Caminetto ver." + String(versione);
  client.publish(logCamiTopic, s.c_str());
  delay(10);
  client.subscribe(systemTopic);
  delay(10);
  client.subscribe(updateTopic);
  delay(10);
  mqttOK=client.subscribe(setupCamiTopic);
  smartDelay(250);
  if(mqttOK)  cs = K_ALL_OK; else ps = K_SLEEP_REQ;
  smartDelay(50);
}

//QUESTO MANDA DEI MESSAGGI MQTT PER DEBUG
void sendAlarm(uint8_t alNr) {
  smartDelay(50);

  String s;
  switch (alNr)
  {
  case 1:
    s = "SHORT TO GROUND";
    break;
  case 2:
    s = "SHORT TO VCC";
    break;
  case 3:
    s = "OPEN CIRCUIT";
    break;
  case 4:
    s = "GENERIC ERROR";
    break;
  case 5:
    s = "NO READ";
    break;
  case 6:
    s = "NO COMMUNICATION";
    break;  
  default:
    break;
  }
  smartDelay(50);
  client.publish(logCamiTopic, "Caminetto errore TC");
  smartDelay(250);
  mqttOK=client.publish(logCamiTopic, s.c_str() );
  smartDelay(50);
  if(mqttOK)  cs = K_ALL_OK; else ps = K_SLEEP_REQ;
}

void wifiOFF(){
  WiFi.disconnect(true);
  delay(10);
  WiFi.mode(WIFI_OFF); //energy saving mode if local WIFI isn't connected
  WiFi.forceSleepBegin();
  delay(10);
}

//MANDO IL WEMOS A NANNA PER UN PO
void adessoDormo(){

  client.publish(logCamiTopic, "Spengo Caminetto");
  client.disconnect();
  delay(10);
  wifiK::wifiOFF();
  delay(10);
  //system_deep_sleep_set_option(2);
  //system_deep_sleep_instant(360000*1000);
  delay(checkWifi);
  ps = K_REBOOT_REQ;
}



//ROUTINE PER CONNESSIONE WIFI
void setupWifi(){
  WiFi.persistent(false);   // Solve possible wifi init errors (re-add at 6.2.1.16 #4044, #4083)
  WiFi.disconnect(true);    // Delete SDK wifi config
  delay(200);
  WiFi.setOutputPower(14);        // 10dBm == 10mW, 14dBm = 25mW, 17dBm = 50mW, 20dBm = 100mW
  WiFi.mode(WIFI_OFF); //energy saving mode if local WIFI isn't connected
  delay(10);
  WiFi.hostname(CamiId);      // DHCP Hostname (useful for finding device for static lease)
  WiFi.mode(WIFI_STA);
  WiFi.forceSleepWake();
  delay(10);
  WiFi.config(ipCaminetto, gateway, subnet,dns1); // Set static IP (2,7s) or 8.6s with DHCP  + 2s on battery
  delay(10);
  WiFi.begin(ssid, password);
}

//ROUTINE PER CONNESSIONE MQTT
void startMqtt()
{
  String clientId = String(CamiId);
  clientId += String(random(0xffff), HEX);
  delay(10);
  //randomSeed(micros());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  delay(100);
  client.connect(clientId.c_str(),mqttUser,mqttPass);
  delay(10);
  wifi_initiate = millis();
  while (!client.connected()) {
    if ((millis() - wifi_initiate) > wifi_check_time) {
      ps = K_SLEEP_REQ;
      return;
    }
    delay(500);
  } 
  reconnect();
  
}

//
void connWifi(){
  wifi_initiate = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if ((millis() - wifi_initiate) > wifi_check_time) {
      ps = K_SLEEP_REQ;
      return;
    }
    delay(500);
  }
  cs = K_WIFI_OK;
}
}