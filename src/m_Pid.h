#pragma once
#include <PID_v1.h>
#include <Arduino.h>
#include <m_def.h>
namespace scPID{

//#define PIN_INPUT 0
#define PIN_OUTPUT D1

//Define Variables we'll be connecting to
double Setpoint , _Input, _Output;
//Specify the links and initial tuning parameters
//double Kp, Ki, Kd; //lento
double Kp=5, Ki=4, Kd=1; //aggressivo
PID myPID(&_Input, &_Output, &Setpoint, Kp, Ki, Kd, REVERSE);

void setupPid()
  {
    //initialize the variables we're linked to
    analogWriteFreq(30000); //frequenza PWM altrimenti la ventola è rumorosa
    delay(10);
    myPID.SetOutputLimits(settaggi.sMinFan, settaggi.sMaxFan);
    delay(10);
    myPID.SetTunings(settaggi.P,settaggi.I,settaggi.D);
    Setpoint = settaggi.sTemp;
    delay(10);
    //turn the PID on
    myPID.SetMode(AUTOMATIC);
    delay(10);
  }

void pidloop()
  {
    _Input = kVal.t_DS;
    myPID.Compute();
    delay(10);
    kVal.pidVal = _Output;
    analogWrite(PIN_OUTPUT,  kVal.pidVal);
    delay(10);
  }
}