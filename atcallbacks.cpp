#include "atcallbacks.h"
#include "strings.h"
#include "oversample.h"
#include "avrio.h"

#include <EEPROM.h>

String processATCommand(const String command, GeneralSettings & settings, ValueData &values)
{
    String param0 = getParam(command, ' ', 0);
    String param1 = getParam(command, ' ', 1);
    String param2 = getParam(command, ' ', 2);            
    
    if (param0 == "I") return printIdGeneral(param1, param2, settings, values);
    if (param0 == "Z") return resetTerminal(param1, param2, settings, values);
    if (param0 == "D") return setDefaults(param1, param2, settings, values);
    if (param0 == "AM0") return disableAutoPinMode(param1, param2, settings, values);
    if (param0 == "AM1") return enableAutoPinMode(param1, param2, settings, values);
    if (param0 == "E0") return turnEchoOff(param1, param2, settings, values);
    if (param0 == "E1") return turnEchoOn(param1, param2, settings, values);
    if (param0 == "L0") return turnLinefeedOff(param1, param2, settings, values);
    if (param0 == "L1") return turnLinefeedOn(param1, param2, settings, values);
    if (param0 == "LA") return loadAnalogOutputsFromEEPROM(param1, param2, settings, values);
    if (param0 == "LD") return loadDigitalOutputsFromEEPROM(param1, param2, settings, values);
    if (param0 == "LS") return loadSettingsFromEEPROM(param1, param2, settings, values);
    if (param0 == "M0") return turnMemoryOff(param1, param2, settings, values);
    if (param0 == "M1") return turnMemoryOn(param1, param2, settings, values);
    if (param0 == "MA") return enableAnalogInputMonitoring(param1, param2, settings, values);
    if (param0 == "MD") return disableAnalogInputMonitoring(param1, param2, settings, values);
    if (param0 == "MI") return monitorAnalogInput(param1, param2, settings, values);
    if (param0 == "MM") return setAnalogMonitorMode(param1, param2, settings, values);
    if (param0 == "OP") return setOversamplePrescaler(param1, param2, settings, values);
    if (param0 == "OR") return setOversampleResolution(param1, param2, settings, values);
    if (param0 == "PD") return setPWMDivisor(param1, param2, settings, values);
    if (param0 == "PS") return printSettings(param1, param2, settings, values);
    if (param0 == "SA") return saveAnalogOutputsToEEPROM(param1, param2, settings, values);
    if (param0 == "SD") return saveDigitalOutputsToEEPROM(param1, param2, settings, values);
    if (param0 == "SI") return setAnalogMonitorInterval(param1, param2, settings, values);
    if (param0 == "SM") return setPINMode(param1, param2, settings, values);
    if (param0 == "SR") return setAnalogReferenceVoltage(param1, param2, settings, values);
    if (param0 == "SV") return setExternalAnalogReferenceVoltage(param1, param2, settings, values);
    if (param0 == "SS") return saveSettingsToEEPROM(param1, param2, settings, values);

    return "Unrecognized AT command";
    
}

// existing AT callbacks

String printIdGeneral(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    return ID_GENERAL;
}

String resetTerminal(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    settings.echoMode = true;
    settings.autoPinMode = false;
    settings.analogMonitorMode = 0;
    settings.linefeed = false;
    settings.memory = true;
    settings.monitorAll = false;
    settings.monitorInterval = 1;
    settings.monitoredAnalogPin = -1;
    settings.analogReferenceOption = 0;
    settings.analogReferenceVoltage = 5.0;
    settings.oversamplePrescaler = 7;
    settings.oversampleResolution = 10;
    settings.pwmDivisors[0] = 3;
    settings.pwmDivisors[1] = 3;
    settings.pwmDivisors[2] = 3;
                
    analogReference(DEFAULT);
    setPrescaler(settings.oversamplePrescaler);
    setPWMDivisor(0, 3);
    setPWMDivisor(1, 3);
    setPWMDivisor(2, 3);
                
    for (int i = 2; i <= 13; i++)
    {
        values.pinModes[i-2] = INPUT;        
        values.digitalOutValues[i-2] = 0;
        pinMode(i, INPUT);
    }
    values.analogOutValues[0] = 0;
    values.analogOutValues[1] = 0;
    values.analogOutValues[2] = 0;
    values.analogOutValues[3] = 0;
    values.analogOutValues[4] = 0;
    values.analogOutValues[5] = 0;
    
    return "Terminal has been reset";            
}

String disableAutoPinMode(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    settings.autoPinMode = false;
    return "OK";    
}

String enableAutoPinMode(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    settings.autoPinMode = true;
    return "OK";    
}

String setDefaults(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    settings.echoMode = true;
    settings.autoPinMode = false;
    settings.analogMonitorMode = 0;
    settings.linefeed = false;
    settings.memory = true;
    settings.monitorInterval = 1;
    settings.analogReferenceOption = 0;
    settings.analogReferenceVoltage = 5.0;
    settings.oversamplePrescaler = 7;
    settings.oversampleResolution = 10;
    settings.pwmDivisors[0] = 3;
    settings.pwmDivisors[1] = 3;
    settings.pwmDivisors[2] = 3;
                
    analogReference(DEFAULT);
    setPrescaler(settings.oversamplePrescaler);
    setPWMDivisor(0, 3);
    setPWMDivisor(1, 3);
    setPWMDivisor(2, 3);
    return "OK";                    
}

String turnEchoOff(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    settings.echoMode = false;
    return "OK";
}

String turnEchoOn(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    settings.echoMode = true;
    return "OK";
}

String turnLinefeedOff(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    settings.linefeed = false;
    return "OK";
}

String turnLinefeedOn(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    settings.linefeed = true;
    return "OK";    
}

String loadAnalogOutputsFromEEPROM(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    if (!settings.memory)
    {
        return "EEPROM is off";
    }
    
    
    int offset = sizeof(GeneralSettings);
    EEPROM.get(offset, values.analogOutValues);
    pinMode(3, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    values.pinModes[3-2] = OUTPUT;
    values.pinModes[5-2] = OUTPUT;
    values.pinModes[6-2] = OUTPUT;
    values.pinModes[9-2] = OUTPUT;
    values.pinModes[10-2] = OUTPUT;
    values.pinModes[11-2] = OUTPUT;
    analogWrite(3, values.analogOutValues[0]);
    analogWrite(5, values.analogOutValues[1]);
    analogWrite(6, values.analogOutValues[2]);
    analogWrite(9, values.analogOutValues[3]);
    analogWrite(10, values.analogOutValues[4]);
    analogWrite(11, values.analogOutValues[5]);
    return "OK";
}

String loadDigitalOutputsFromEEPROM(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    if (!settings.memory)
    {
        return "EEPROM is off";
    }
    
    int offset = sizeof(GeneralSettings) + 6 * sizeof(byte);
    EEPROM.get(offset, values.digitalOutValues);

    for (int i = 2; i <= 13; i++)
    {
        pinMode(i, OUTPUT);
        values.pinModes[i-2] = OUTPUT;
        digitalWrite(i, values.digitalOutValues[i-2]);
    }
    
    return "OK";
}

String loadSettingsFromEEPROM(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    if (!settings.memory)
    {
        return "EEPROM is off";
    }    
    //load settings
    EEPROM.get(0, settings);
    if (settings.analogReferenceOption == 0) 
    {
        analogReference(DEFAULT);                        
    }    
    else if (settings.analogReferenceOption == 1)
    {
        analogReference(INTERNAL);
    }
    else
    {
        analogReference(EXTERNAL);
    }
    setPrescaler(settings.oversamplePrescaler);
    setPWMDivisor(0, settings.pwmDivisors[0]);
    setPWMDivisor(1, settings.pwmDivisors[1]);
    setPWMDivisor(2, settings.pwmDivisors[2]);
    
    return "OK";
}

String turnMemoryOff(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    settings.memory = false;
    return "OK";    
}

String turnMemoryOn(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    settings.memory = true;
    return "OK";    
}

String enableAnalogInputMonitoring(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    settings.monitorAll = true;
    settings.monitoredAnalogPin = -1;
    return "OK";
}

String disableAnalogInputMonitoring(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    settings.monitorAll = false;
    settings.monitoredAnalogPin = -1;
    return "OK";    
}

String monitorAnalogInput(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    if (param1.length() != 2)
    {
        return "Empty or Invalid param value";
    }
    int pinNum = hex2int(param1, 2);
    settings.monitoredAnalogPin = pinNum;
    return "OK";    
}

String setAnalogMonitorMode(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
   if (param1 == "A")
   {
       settings.analogMonitorMode = 0;
       return "OK";
   }
   else if (param1 == "D")
   {
       settings.analogMonitorMode = 1;
       return "OK";
   }
   else if (param1 == "V")
   {
       settings.analogMonitorMode = 2;
       return "OK";
   }
   else
   {
       return "Unrecognized AT command";
   }
}

String setOversamplePrescaler(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    if (param1.length() != 1)
    {
        return "Empty or Invalid param value";
    }
    
    byte prescaler = (byte) param1.toInt();
    if (prescaler > 7)
    {
        return "Invalid oversample prescaler value";
    }
    settings.oversamplePrescaler = prescaler;
    setPrescaler(prescaler);
    return "OK";      
}

String setOversampleResolution(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    if (param1.length() != 2)
    {
        return "Empty or Invalid param value";
    }
    
    byte resolution = (byte) hex2int(param1, 2);
    if ((resolution < 10) || (resolution > 16))
    {
        return "Invalid oversample resolution value";
    }
    settings.oversampleResolution = resolution;
    settings.monitorInterval = 1000;
    return "OK";    
}

String setPWMDivisor(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    if (param1.length() != 2)
    {
        return "Empty or Invalid param value";
    }
    
    byte timer = (byte) param1.substring(0, 1).toInt();
    byte divisor = (byte) param1.substring(1, 2).toInt();
    if (timer > 2)
    {
        return "Invalid timer number";
    }
    if (((timer == 0) || (timer == 1)) && ((divisor < 1) || (divisor > 5)))
    {
        return "Invalid divisor value for timer " + String(timer);
    }
    if ((timer == 2) && ((divisor < 1) || (divisor > 7)))
    {
        return "Invalid divisor value for timer " + String(timer);
    }
    settings.pwmDivisors[timer] = divisor;
    setPWMDivisor(timer, divisor);
    return "OK";    
}

String printSettings(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    String arefopt;
    switch (settings.analogReferenceOption)
    {
        case 0: arefopt = "DEFAULT"; break;
        case 1: arefopt = "INTERNAL"; break;
        case 2: arefopt = "EXTERNAL"; break;
        default: arefopt = "DEFAULT"; break;
    }
    String result = "";
    result += "ECHO          : " + boolToString(settings.echoMode, "ON", "OFF") + "\n";
    result += "AUTO PIN MODE : " + boolToString(settings.autoPinMode, "ON", "OFF") + "\n";
    result += "LINE FEED     : " + boolToString(settings.linefeed, "ON", "OFF") + "\n";
    result += "MEMORY        : " + boolToString(settings.memory, "ON", "OFF") + "\n";
    result += "AREF VOLTAGE  : " + floatToString(settings.analogReferenceVoltage) + " V\n";
    result += "AREF MODE     : " + arefopt + "\n";
    result += "MON. INTERVAL : " + String(settings.monitorInterval) + " ms";
    return result;
}

String saveAnalogOutputsToEEPROM(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    if (!settings.memory)
    {
        return "EEPROM is off";
    }
    int offset = sizeof(GeneralSettings);
    EEPROM.put(offset, values.analogOutValues);
    return "OK";
}

String saveDigitalOutputsToEEPROM(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    if (!settings.memory)
    {
        return "EEPROM is off";
    }
    int offset = sizeof(GeneralSettings) + 6 * sizeof(byte);
    EEPROM.put(offset, values.digitalOutValues);
    return "OK";
}

String setAnalogMonitorInterval(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    if (param1.length() != 4)
    {
        return "Empty or Invalid param value";
    }
    
    settings.monitorInterval = (unsigned int) hex2int(param1, 4);
    if (settings.monitorInterval == 0)
    {
        settings.monitorInterval = 1;
        return "Illegal monitor interval";
    }
    return "OK";    
}

String setPINMode(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    if (param1.length() != 2)
    {
        return "Empty or Invalid param1 value";
    }
    if (param2.length() != 1)
    {
        return "Empty or Invalid param2 value";
    }
    
    String pin_hex = param1;
    byte pinNum = (byte) hex2int(pin_hex, 2);
    if ((pinNum < 2) || (pinNum > 13))
    {
        return "Illegal pin number";
    }
    String modeValue = param2;
    modeValue.toUpperCase();
    if (modeValue == "I")
    {
        pinMode(pinNum, INPUT);
        values.pinModes[pinNum-2] = INPUT;
        return "OK";
    }
    else if (modeValue == "O")
    {
        pinMode(pinNum, OUTPUT);
        values.pinModes[pinNum-2] = OUTPUT;
        return "OK";
    }
    else if (modeValue == "P")
    {
        pinMode(pinNum, INPUT_PULLUP);
        values.pinModes[pinNum-2] = INPUT_PULLUP;
        return "OK";
    }
    else
    {
        return "Unrecognized pin setup option";                
    }
}

String setAnalogReferenceVoltage(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    if (param1.length() != 1)
    {
        return "Empty or Invalid param value";
    }
    
    if (param1 == "D")
    {
        settings.analogReferenceOption = 0;
        settings.analogReferenceVoltage = 5.0;
        analogReference(DEFAULT);
        return "OK";
    }
    if (param1 == "I")
    {
        settings.analogReferenceOption = 1;
        settings.analogReferenceVoltage = 1.1;
        analogReference(INTERNAL);
        return "OK";
    }
    if (param1 == "E")
    {
        settings.analogReferenceOption = 2;                    
        analogReference(EXTERNAL);
        return "OK";
    }
    return "Unrecognized AT command";    
}

String setExternalAnalogReferenceVoltage(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    if (param1.length() != 4)
    {
        return "Empty or Invalid param value";
    }
    
    unsigned int aref = hex2int(param1, 4);
    if (aref > 5000)
    {
        return "Illegal external ARef voltage";
    }    

    settings.analogReferenceVoltage = (float) aref / 1000.0;
    settings.analogReferenceOption = 2;
    analogReference(EXTERNAL);
    return "OK";         
}

String saveSettingsToEEPROM(const String param1, const String param2, GeneralSettings & settings, ValueData &values)
{
    if (!settings.memory)
    {
        return "EEPROM is off";
    }
    EEPROM.put(0, settings);
    return "OK";
}


