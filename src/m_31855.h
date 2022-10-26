#include "MAX31855.h"
namespace sondaK{
const uint8_t doPin = D5;
const uint8_t csPin = D6;
const uint8_t clPin = D7;
uint8_t errNr = 0;
MAX31855 tc;
float offset ;

void setupK() 
{
  // Serial.begin(9600);
  // Serial.print("Start max31855_test_error: ");
  // Serial.println(MAX31855_VERSION);
  // Serial.println();

  tc.begin(clPin, csPin, doPin);
  tc.setOffset(offset);
}


float getTemperature() {

  int status = tc.read();
  // Serial.print("stat:\t\t");
  // Serial.println(status);
  delay(10);
  if (tc.getStatus())
  {
    Serial.print("error:\t\t");
    if (tc.shortToGND())   errNr = 1; //Serial.println("SHORT TO GROUND");
    if (tc.shortToVCC())   errNr = 2; //Serial.println("SHORT TO VCC");
    if (tc.openCircuit())  errNr = 3; //Serial.println("OPEN CIRCUIT");
    if (tc.genericError()) errNr = 4; //Serial.println("GENERIC ERROR");
    if (tc.noRead())       errNr = 5; //Serial.println("NO READ");
    if (tc.noCommunication()) errNr = 6; //Serial.println("NO COMMUNICATION");
  }
//  {
//     Serial.print("error:\t\t");
//     if (tc.shortToGND())   Serial.println("SHORT TO GROUND");
//     if (tc.shortToVCC())   Serial.println("SHORT TO VCC");
//     if (tc.openCircuit())  Serial.println("OPEN CIRCUIT");
//     if (tc.genericError()) Serial.println("GENERIC ERROR");
//     if (tc.noRead())       Serial.println("NO READ");
//     if (tc.noCommunication()) Serial.println("NO COMMUNICATION");
//   }
  // float internal = tc.getInternal();
  // Serial.print("internal:\t");
  // Serial.println(internal, 3);
  //tc.getStatus();
  float temp = tc.getTemperature();

  
  // Serial.print("temperature:\t");
  // Serial.println(temp, 2);
  
  return temp;
}

}

// -- END OF FILE --