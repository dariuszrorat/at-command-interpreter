#include <math.h>
#include "oversample.h"

void setPrescaler(byte value)
{
  byte mask = B11111000;
  ADCSRA &= mask;
  ADCSRA |= value;
}

unsigned int analogReadOversampled(byte pin, GeneralSettings settings)
{
  unsigned long total = 0UL;
  byte additionalBits = settings.oversampleResolution - 10;
  int numsamples = B00000001 << (additionalBits * 2);
  for(int i = 0; i < numsamples; i++) 
  {
    total += analogRead(pin);
  }

  return total >> additionalBits;    
}

float analogReadOversampledVoltage(byte pin, GeneralSettings settings)
{
    float maxvalue = pow(2, (float) settings.oversampleResolution) - 1;
    return (float) analogReadOversampled(pin, settings) * settings.analogReferenceVoltage / maxvalue;
}


