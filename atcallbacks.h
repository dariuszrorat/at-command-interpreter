#ifndef ATCALLBACKS_H
#define ATCALLBACKS_H

#define ID_GENERAL "Arduino Uno R3"

#include "typedefs.h"


String processATCommand(const String command, GeneralSettings & settings, ValueData &values);

// existing AT callbacks
String printIdGeneral(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String resetTerminal(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String disableAutoPinMode(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String enableAutoPinMode(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String setDefaults(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String turnEchoOff(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String turnEchoOn(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String turnLinefeedOff(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String turnLinefeedOn(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String loadAnalogOutputsFromEEPROM(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String loadDigitalOutputsFromEEPROM(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String loadSettingsFromEEPROM(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String turnMemoryOff(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String turnMemoryOn(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String enableAnalogInputMonitoring(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String disableAnalogInputMonitoring(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String monitorAnalogInput(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String setAnalogMonitorMode(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String setOversamplePrescaler(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String setOversampleResolution(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String setPWMDivisor(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String printSettings(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String saveAnalogOutputsToEEPROM(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String saveDigitalOutputsToEEPROM(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String setAnalogMonitorInterval(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String setPINMode(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String setAnalogReferenceVoltage(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String setExternalAnalogReferenceVoltage(const String param1, const String param2, GeneralSettings & settings, ValueData &values);
String saveSettingsToEEPROM(const String param1, const String param2, GeneralSettings & settings, ValueData &values);

#endif

