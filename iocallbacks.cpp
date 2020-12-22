#include "iocallbacks.h"
#include "strings.h"
#include "avrio.h"
#include "oversample.h"

byte resolveAnalogPinIndex(byte pin)
{
    switch (pin)
    {
        case 3: return 0;    
        case 5: return 1;
        case 6: return 2;
        case 9: return 3;
        case 10: return 4;
        case 11: return 5;
        default: return 0;
    }
}

String processIOCommand(const String command, GeneralSettings settings, ValueData &values)
{
    String param0 = getParam(command, ' ', 0);
    String param1 = getParam(command, ' ', 1);
    String param2 = getParam(command, ' ', 2);
    String param3 = getParam(command, ' ', 3);            
    
    if (param0 == "00") return analogReadInput(param1, param2, param3, settings, values);
    if (param0 == "01") return analogReadInputVoltage(param1, param2, param3, settings, values);
    if (param0 == "02") return analogReadInputAsDigital(param1, param2, param3, settings, values);
    if (param0 == "10") return digitalReadInput(param1, param2, param3, settings, values);
    if (param0 == "11") return digitalReadPINState(param1, param2, param3, settings, values);
    if (param0 == "13") return readPulseDuration(param1, param2, param3, settings, values);
    if (param0 == "20") return digitalWriteOutput(param1, param2, param3, settings, values);
    if (param0 == "21") return analogWriteOutput(param1, param2, param3, settings, values);
    if (param0 == "30") return generateSquareWaveOutput(param1, param2, param3, settings, values);
    if (param0 == "31") return generateSquareWaveOutputTime(param1, param2, param3, settings, values);
    if (param0 == "32") return disableToneGeneration(param1, param2, param3, settings, values);
    
    return "Unrecognized I/O command";
}

// existing IO callbacks
String analogReadInput(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values)
{
    if (param1.length() != 2)
    {
        return "Empty or Invalid param value";
    }
    
    byte pin;
    unsigned int input;
    String stringVal = "";
    
    pin = (byte) hex2int(param1, 2);
    input = (settings.oversampleResolution > 10) ? analogReadOversampled(pin, settings) : analogRead(pin);
    stringVal = String(input);
                
    return stringVal;
}

String analogReadInputVoltage(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values)
{
    if (param1.length() != 2)
    {
        return "Empty or Invalid param value";
    }
    
    byte pin;
    unsigned int input;
    String stringVal = "";
    float voltage = 0;
   
    pin = (byte) hex2int(param1, 2);
    if (settings.oversampleResolution > 10)
    {
        voltage = analogReadOversampledVoltage(pin + 14, settings); // A0..A5
        char buffer[20];
        dtostrf(voltage, 10, 6, buffer);
        stringVal = String(buffer);
        return stringVal;                    
    }
    else
    {
        voltage = (float) analogRead(pin) * settings.analogReferenceVoltage / 1023.0  ;
        stringVal = floatToString(voltage);                
        return stringVal;
    }    
}

String analogReadInputAsDigital(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values)
{
    if (param1.length() != 2)
    {
        return "Empty or Invalid param value";
    }
    
    byte pin;
    unsigned int input;
    String stringVal = "";

    pin = (byte) hex2int(param1, 2);
    input = digitalRead(pin + 14); // A0..A5
    stringVal = String(input);
    return stringVal;
}

String digitalReadInput(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values)
{
    if (param1.length() != 2)
    {
        return "Empty or Invalid param value";
    }
    
    byte pin;
    unsigned int input;
    bool digitalInput;
    String stringVal = "";

    pin = (byte) hex2int(param1, 2);
    if ((pin < 2) || (pin > 13))
    {
        return "Invalid digital PIN number";
    }
    if (settings.autoPinMode)
    {
        pinMode(pin, INPUT);
        values.pinModes[pin-2] = INPUT;
    }
    
    digitalInput = digitalRead(pin);
    stringVal = digitalInput ? "1" : "0";
    return stringVal;
}

String digitalReadPINState(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values)
{
    if (param1.length() != 2)
    {
        return "Empty or Invalid param value";
    }
    
    byte pin;
    unsigned int input;
    bool digitalInput;
    bool digitalOutput;
    String stringVal = "";

    pin = (byte) hex2int(param1, 2);
    if ((pin < 2) || (pin > 13))
    {
        return "Invalid digital PIN number";
    }
    if (settings.autoPinMode)
    {
        pinMode(pin, INPUT);
        values.pinModes[pin-2] = INPUT;
    }

    digitalOutput = digitalReadOutputPin(pin);
    stringVal = digitalOutput ? "1" : "0";
    return stringVal;
}

String readPulseDuration(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values)
{
    if (param1.length() != 2)
    {
        return "Empty or Invalid param1 value";
    }

    if (param2.length() != 1)
    {
        return "Empty or Invalid param2 value";
    }

    if (param3.length() != 2)
    {
        return "Empty or Invalid param3 value";
    }
    
    
    byte pin;
    unsigned int input;
    byte state;
    unsigned long timeout;
    unsigned long duration;
    bool digitalInput;
    bool digitalOutput;
    String stringVal = "";

    pin = (byte) hex2int(param1, 2);
    if ((pin < 2) || (pin > 13))
    {
        return "Invalid digital PIN number";
    }
    state = (byte) param2.toInt();
    timeout = (unsigned long) (hex2int(param3, 2) * 1000000);
    duration = pulseIn(pin, state, timeout); // D2..D13
    stringVal = String(duration);
    return stringVal;
}

String digitalWriteOutput(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values)
{
    if (param1.length() != 2)
    {
        return "Empty or Invalid param1 value";
    }
    if (param2.length() != 1)
    {
        return "Empty or Invalid param value";
    }
    
    
    byte pin;
    unsigned int input;
    bool digitalInput;
    bool digitalOutput;
    String stringVal = "";

    pin = (byte) hex2int(param1, 2);
    if ((pin < 2) || (pin > 13))
    {
        return "Invalid digital PIN number";
    }
    String parValue = param2;
    byte outValue = parValue.toInt();

    if (settings.autoPinMode)
    {
        pinMode(pin, OUTPUT);
        values.pinModes[pin-2] = OUTPUT;
    }
    else
    {
        if (values.pinModes[pin-2] != OUTPUT)
        {
            return "Invalid PIN mode";
        }
    }
    if (outValue == 0)
    {
        digitalWrite(pin, LOW);
    }
    else
    {
        digitalWrite(pin, HIGH);
    }
    values.digitalOutValues[pin-2] = outValue;
    digitalOutput = digitalReadOutputPin(pin);
    return String((int) digitalOutput);
}

String analogWriteOutput(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values)
{
    if (param1.length() != 2)
    {
        return "Empty or Invalid param1 value";
    }
    if (param2.length() != 2)
    {
        return "Empty or Invalid param2 value";
    }
    
    
    byte pin;
    unsigned int input;
    bool digitalInput;
    bool digitalOutput;
    String stringVal = "";
    byte pinIndex;

    pin = (byte) hex2int(param1, 2);
    if ((pin != 3) && (pin != 5) && (pin != 6) && (pin != 9) && (pin != 10) && (pin != 11))
    {
        return "Not PWM pin";
    }

    String hexValue = param2;
    byte outValue = hex2int(hexValue, 2);
    pinIndex = resolveAnalogPinIndex(pin);    
    values.analogOutValues[pinIndex] = outValue;
    if (settings.autoPinMode)
    {
        pinMode(pin, OUTPUT);
        values.pinModes[pin-2] = OUTPUT;
    }
    else
    {
        if (values.pinModes[pin-2] != OUTPUT)
        {
            return "Invalid PIN mode";
        }
    }
    analogWrite(pin, outValue);
    return String(outValue);
}

String generateSquareWaveOutput(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values)
{
    if (param1.length() != 2)
    {
        return "Empty or Invalid param1 value";
    }
    if (param2.length() != 4)
    {
        return "Empty or Invalid param2 value";
    }
    
    
    byte pin;
    unsigned int input;
    bool digitalInput;
    bool digitalOutput;
    String stringVal = "";

    pin = (byte) hex2int(param1, 2);
    if ((pin != 3) && (pin != 5) && (pin != 6) && (pin != 9) && (pin != 10) && (pin != 11))
    {
        return "Not PWM pin";
    }
    String hexValue = param2;
    unsigned int outValue = hex2int(hexValue, 4);
    if (settings.autoPinMode)
    {
        pinMode(pin, OUTPUT);
        values.pinModes[pin-2] = OUTPUT;
    }
    else
    {
        if (values.pinModes[pin-2] != OUTPUT)
        {
            return "\nInvalid PIN mode";
        }
    }
    tone(pin, outValue);
    return String(outValue) + " Hz";
}

String generateSquareWaveOutputTime(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values)
{
    if (param1.length() != 2)
    {
        return "Empty or Invalid param1 value";
    }
    if (param2.length() != 4)
    {
        return "Empty or Invalid param2 value";
    }
    if (param3.length() != 4)
    {
        return "Empty or Invalid param3 value";
    }
    
    byte pin;
    unsigned int input;
    bool digitalInput;
    bool digitalOutput;
    String stringVal = "";

    pin = (byte) hex2int(param1, 2);
    if ((pin != 3) && (pin != 5) && (pin != 6) && (pin != 9) && (pin != 10) && (pin != 11))
    {
        return "Not PWM pin";
    }
    String hexValue = param2;
    String hexDuration = param3;
    int outValue = hex2int(hexValue, 4);
    int duration = hex2int(hexDuration, 4);
    if (settings.autoPinMode)
    {
        pinMode(pin, OUTPUT);
        values.pinModes[pin-2] = OUTPUT;
    }
    else
    {
        if (values.pinModes[pin-2] != OUTPUT)
        {
            return "Invalid PIN mode";
        }
    }
    tone(pin, outValue, duration);
    return String(outValue) + " Hz, " + String(duration) + " ms";
}

String disableToneGeneration(const String param1, const String param2, const String param3, GeneralSettings settings, ValueData &values)
{
    if (param1.length() != 2)
    {
        return "Empty or Invalid param value";
    }
    
    byte pin;
    unsigned int input;
    bool digitalInput;
    bool digitalOutput;
    String stringVal = "";

    pin = (byte) hex2int(param1, 2);
    if ((pin != 3) && (pin != 5) && (pin != 6) && (pin != 9) && (pin != 10) && (pin != 11))
    {
        return "Not PWM pin";
    }
    if (settings.autoPinMode)
    {
        pinMode(pin, OUTPUT);
        values.pinModes[pin-2] = OUTPUT;
    }
    else
    {
        if (values.pinModes[pin-2] != OUTPUT)
        {
            return "Invalid PIN mode";
        }
    }

    noTone(pin);
    return "Tone was disabled";
}


