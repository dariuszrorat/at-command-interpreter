/*
    AT Command interpreter for Arduino Uno R3

    AT commands:

    AT                 repeat the last command
    AT I               print ID general
    AT Z               reset terminal
    AT D               set all to defaults
    AT AM0             disable auto pin mode
    AT AM1             enable auto pin mode
    AT E0              turn echo off
    AT E1              turn echo on
    AT L0              linefeed off
    AT L1              linefeed on
    AT LA              load analog output values from EEPROM
    AT LD              load digital output values from EEPROM
    AT LS              load stored settings from EEPROM memory
    AT M0              memory off
    AT M1              memory on
    AT MA              monitor all analog inputs
    AT MD              disable analog input monitor
    AT MI XX           monitor XX analog input, XX must be 00..05
    AT MM X            set analog monitor mode, A = analog read, D = digital read, V = analog voltage read
    AT OP X            set oversample prescaler, X must be 0..7
    AT OR XX           set oversample ADC resolution, XX must be 0A..10  
    AT PD XY           set PWM timer divisor, X=0 (D5, D6), 1 (D9, D10), 2 (D3, D11), X=0..1 => Y=1..5 and X=2 => Y=1..7
    AT PS              print settings
    AT SA              store analog output value at PWM output to EEPROM
    AT SD              store digital output value to EEPROM
    AT SI TTTT         set analog monitor interval in ms, TTTT = 0001..FFFF
    AT SM XX Y         set XX pin mode (hex pin number) as Y where Y is I = input, O = output, P = input pullup
    AT SR X            set analog reference voltage as X where X: D = default 5V, I = internal 1.1V, E = external using ARef pin
    AT SV XXXX         set external analog reference voltage value, auto set external reference voltage, use ARef pin
    AT SS              store settings to EEPROM memory

    I/O commands:

    00 XX              read analog input (where XX is between 00..05 for Arduino Uno), example: 00 00 reads A0 analog input
    01 XX              read analog input voltage (where XX is between 00..05 for Uno), example: 01 00 reads A0 analog input voltage
    02 XX              read analog input as digital value
    10 XX              read digital input (XX must between 02..0D as hex for Uno R3)
    11 XX              read digital pin state
    13 XX Y TT         read pulse duration from digital input XX, Y is state, TT is timeout in sec, XX = 02..0D, Y = 0 or 1, TT = 00..FF
    20 XX Y            write value to digital output (XX must be between 02..0D for Uno R3), Y is 0 or 1, 0 = LOW, 1 = HIGH
    21 XX YY           write analog output, XX must be PWM output as hex, YY must be 00..FF
    30 XX YYYY         generate squarewave at PWM output XX, YYYY is frequency as hex value
    31 XX YYYY ZZZZ    generate squarewave at PWM output XX, YYYY is frequency as hex value, ZZZZ is generation time in msec as hex
    32 XX              disable tone generation at PWM XX output
*/

#include "typedefs.h"
#include "atcallbacks.h"
#include "iocallbacks.h"
#include "oversample.h"

String inputString = "";
String outputString = "";
boolean stringComplete = false;
GeneralSettings settings;
ValueData values;

unsigned int analogInput;
float analogVoltage;
unsigned int counter = 0;

void setup()
{
    Serial.begin(9600);
    inputString.reserve(200);
    Serial.print(">");

    //initialize general settings
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
}

void loop()
{    
    if (stringComplete)
    {        
        inputString.trim();
        inputString.toUpperCase();        
        doCommand(inputString);
        
        // clear the string:
        inputString = "";
        stringComplete = false;        
    }

    doMonitoring();        
}

// private methods

void doCommand(String input)
{
    if (settings.echoMode)
    {
        Serial.println(input);    
    }
    
    if (input.length() < 2)
    {
        Serial.println("Unrecognized command");
        Serial.print(">");
        return;
    }
    
    String id = input.substring(0, 2);
    String params;
    String answer;
    
    if (id == "AT")
    {
        params = input.substring(2);        
        params.trim();
        answer = processATCommand(params, settings, values);
    }
    else
    {
        answer = processIOCommand(input, settings, values);
    }

    Serial.println(answer);
    Serial.print(">");                        
}

void doMonitoring()
{
    if (settings.monitorAll)
    {
        if ((counter % settings.monitorInterval) == 0)
        {

            switch (settings.analogMonitorMode)
            {
            case 0:
            {
                for (byte p = 0; p < 6; p++)
                {
                    analogInput = (settings.oversampleResolution > 10) ? analogReadOversampled(p + 14, settings) : analogRead(p + 14);                    
                    Serial.print("A" + String(p) + ": ");
                    Serial.println(analogInput);
                }
            }
            break;
            case 1:
            {
                for (byte p = 0; p < 6; p++)
                {
                    analogInput = digitalRead(p + 14);
                    Serial.print("A" + String(p) + ": ");
                    Serial.println(analogInput);
                }

            }
            break;
            case 2:
            {
                for (byte p = 0; p < 6; p++)
                {
                    analogVoltage = (settings.oversampleResolution > 10) ? analogReadOversampledVoltage(p + 14, settings) 
                        : (float) analogRead(p + 14) * settings.analogReferenceVoltage / 1023.0;
                    Serial.print("A" + String(p) + ": ");
                    int places = settings.oversampleResolution > 10 ? 6 : 2;
                    Serial.println(analogVoltage, places);                    
                }
            }
            break;
            }
        }
        delay(1);
    }
    else
    {
        if (settings.monitoredAnalogPin >= 0)
        {
            if ((counter % settings.monitorInterval) == 0)
            {
                switch (settings.analogMonitorMode)
                {
                case 0:
                {
                    analogInput = (settings.oversampleResolution > 10) ? analogReadOversampled(settings.monitoredAnalogPin, settings) : analogRead(settings.monitoredAnalogPin);                    
                    Serial.print("A" + String(settings.monitoredAnalogPin) + ": ");
                    Serial.println(analogInput);
                }
                break;
                case 1:
                {
                    analogInput = digitalRead(settings.monitoredAnalogPin + 14) ;
                    Serial.print("A" + String(settings.monitoredAnalogPin) + ": ");
                    Serial.println(analogInput);
                }
                break;
                case 2:
                {
                    analogVoltage = (settings.oversampleResolution > 10) ? analogReadOversampledVoltage(settings.monitoredAnalogPin, settings) 
                        : (float) analogRead(settings.monitoredAnalogPin) * settings.analogReferenceVoltage / 1023.0;
                    Serial.print("A" + String(settings.monitoredAnalogPin) + ": ");
                    int places = settings.oversampleResolution > 10 ? 6 : 2;
                    Serial.println(analogVoltage, places);                                                            
                }
                break;
                }
            }
        }
        delay(1);
    }
    counter++;    

}

void serialEvent() 
{
  char endChar = settings.linefeed ? '\n' : '\r';
  while (Serial.available()) 
  {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == endChar)
    {
      stringComplete = true;
    }    
  }
}


