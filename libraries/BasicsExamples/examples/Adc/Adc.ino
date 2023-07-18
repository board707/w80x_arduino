#include "Arduino.h"
/*
 * HLK-W80x Analog read example.
 * 
 * W80x boards has four analog ports.
 * You can access it from Arduino code by pin numbers PA1-PA4
 * or by short names A1-A4.
 * 
 * Name A0 is alias to A1, added for compatibility.
 * 
 */
void setup()
{
pinMode(A2,ANALOG_INPUT);  // or pinMode(PA2,ANALOG_INPUT);
Serial.begin(115200);
}


void loop()
{
float volt = 0;
volt = analogRead(A2);
Serial.printf("ADC millivolt [%f]\r\n", volt);
delay (1000);
}


