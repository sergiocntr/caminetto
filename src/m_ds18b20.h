#include <OneWire.h>
#include <DallasTemperature.h>
#include <m_def.h>
#include "MedianFilterLib.h"
MedianFilter<float> medianFilterDS(10);
const uint8_t ONE_WIRE_BUS = D3;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
namespace sondaDS{

void setupDS()
{
 DS18B20.begin();
}

void getTemperature() {
  float temp=22.2;
  DS18B20.requestTemperatures();
  delay(10);
  temp = DS18B20.getTempCByIndex(0);
  delay(10);
  if(temp == 85.0 || temp == (-127.0)){temp=22.2;}
    temp =(temp * 5) / 5;
  delay(10);
  //Serial.println(temp);
  kVal.t_DS= medianFilterDS.AddValue(temp);
  delay(10);
}

}