#include "Arduino.h"
#include "SoftwareI2C.h"

SoftwareI2C Wire;
uint8_t dDev = 0;

void setup() {
Wire.begin(PA12, PA14); // sda, scl
printf("begin to scan...\n\r");
}

void loop() {
for (unsigned char i = 0; i <= 127; i++) {
if (Wire.beginTransmission(i)) {
printf("Found i2c device at address [0x%X] \n\r",i);
dDev++;
}
Wire.endTransmission();
}
if (dDev == 0) printf("find nothing");
while (1);
}
