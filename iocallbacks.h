#ifndef IOCALLBACKS_H
#define IOCALLBACKS_H

#include "typedefs.h"

byte resolveAnalogPinIndex(byte pin);

String processIOCommand(const String command, GeneralSettings settings, ValueData &values);

// existing IO callbacks
String analogReadInput(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values);
String analogReadInputVoltage(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values);
String analogReadInputAsDigital(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values);
String digitalReadInput(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values);
String digitalReadPINState(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values);
String readPulseDuration(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values);
String digitalWriteOutput(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values);
String analogWriteOutput(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values);
String generateSquareWaveOutput(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values);
String generateSquareWaveOutputTime(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values);
String disableToneGeneration(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values);

#endif

