#ifndef OVERSAMPLE_H
#define OVERSAMPLE_H

#include <arduino.h>
#include "typedefs.h"

void setPrescaler(byte value);
unsigned int analogReadOversampled(byte pin, GeneralSettings settings);
float analogReadOversampledVoltage(byte pin, GeneralSettings settings);

#endif

