#include "Arduino.h"

void setup()
{
pinMode(A2,ANALOG_INPUT);
}


void loop()
{
float volt = 0;
volt = analogRead(A2);
printf("ADC millivolt [%f]\r\n", volt);
delay (1000);
}

