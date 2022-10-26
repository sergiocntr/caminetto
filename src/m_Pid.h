#pragma once
#include <PID_v1.h>
#include <Arduino.h>
namespace scPID{

//#define PIN_INPUT 0
#define PIN_OUTPUT D1

//Define Variables we'll be connecting to
double Setpoint, _Input, _Output;
double pid_min ;
double pid_max ;
//Specify the links and initial tuning parameters
double Kp=1, Ki=0.05, Kd=0.25;
//double Kp=5, Ki=4, Kd=1;
PID myPID(&_Input, &_Output, &Setpoint, Kp, Ki, Kd, REVERSE);

void setupPid()
{
  //initialize the variables we're linked to
  //Input = analogRead(PIN_INPUT);
  analogWriteFreq(30000); //frequenza PWM altrimenti la ventola è rumorosa
  //Setpoint = 10;
  myPID.SetOutputLimits(pid_min, pid_max);
  delay(10);

//turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void pidloop(double Input)
  {
    _Input = Input;
    myPID.Compute();
    delay(10);
    //_Output = 255 - _Output;
    analogWrite(PIN_OUTPUT,  _Output);
  }


}