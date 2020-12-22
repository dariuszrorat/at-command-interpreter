# AT Command interpreter

This directory contains simple AT and I/O command interpreter.

### Features

  - control devices with a computer via USB
  - measurement data acquisition
  - process automation
  - diagnostics

### AT commands

    AT I               print ID general
    AT Z               reset terminal
    AT AM0             disable auto pin mode
    AT AM1             enable auto pin mode
    AT D               set all to defaults
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
    AT PR              print analog voltage reference voltage
    AT PS              print settings
    AT SA              store analog output value at PWM output to EEPROM
    AT SD              store digital output value to EEPROM
    AT SI TTTT         set analog monitor interval in ms, TTTT = 0001..FFFF
    AT SM HH I         set HH pin mode (hex pin value) as input
    AT SM HH O         set HH pin mode (hex pin value) as output
    AT SM HH P         set HH pin mode (hex pin value) as input pullup
    AT SR X            set analog reference voltage, D = default 5V, I = internal 1.1V, E = external using ARef pin
    AT SV XXXX         set external analog reference voltage value, auto set external reference voltage, use ARef pin
    AT SS              store settings to EEPROM memory

### I/O commands

    00 XX              read analog input (where XX is between 00..05 for Arduino Uno), example: 00 00 reads A0 analog input
    01 XX              read analog input voltage (where XX is between 00..05 for Uno), example: 01 00 reads A0 analog input voltage
    02 XX              read analog input as digital value
    10 XX              read digital input (XX must between 02..0D for Uno R3)
    11 XX              read digital pin state
    13 XX Y TT         read pulse duration from digital input XX, Y is state, TT is timeout in sec, XX = 02..0D, Y = 0 or 1, TT = 00..FF
    20 XX 0            write LOW value to digital output (XX must be between 02..0D for Uno R3)
    20 XX 1            write HIGH value to digital output (XX must be between 02..0D for Uno R3)
    21 XX YY           write analog output, XX must be PWM output as hex, YY must be 00..FF
    30 XX YYYY         generate squarewave at PWM output XX, YYYY is frequency as hex value
    31 XX YYYY ZZZZ    generate squarewave at PWM output XX, YYYY is frequency as hex value, ZZZZ is generation time in msec as hex
    32 XX              disable tone generation at PWM XX output

### Defaults

  - echo is enabled by default
  - linefeed is off (Carriage Return as ending char)
  - EEPROM memory is on
  - monitor is off
  - auto pin mode for digital pins 02..13 (0x02..0x0D) is set to false
  - analog reference is set to default 5V
  - oversample resolution is 10 bit (do not use oversampling)
  - oversample prescaler is 7 (Arduino Default 128)