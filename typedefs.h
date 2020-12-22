#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <arduino.h>

struct GeneralSettings
{
    bool echoMode;
    bool autoPinMode;
    byte analogMonitorMode;
    bool linefeed;
    bool monitorAll;
    bool memory;
    //monitoring
    int monitoredAnalogPin;
    float analogReferenceVoltage;
    byte analogReferenceOption;
    unsigned int monitorInterval;
    //oversampling
    byte oversamplePrescaler;
    byte oversampleResolution;
    //pwm divisors
    byte pwmDivisors[3];
};

struct ValueData
{
    byte pinModes[12];
    byte digitalOutValues[12];
    byte analogOutValues[6];    
};

#endif

