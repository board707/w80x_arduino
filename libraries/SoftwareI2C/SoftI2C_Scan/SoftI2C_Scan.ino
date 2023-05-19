#include "Arduino.h"
#include "SoftwareI2C.h"

SoftwareI2C sWire;
uint8_t dDev = 0;

void setup() {
sWire.begin(PA12, PA14); // sda, scl
printf("begin to scan...\n\r");
}

void loop() {
for (unsigned char i = 0; i <= 127; i++) {
if (sWire.beginTransmission(i)) {
printf("Found i2c device at address [0x%X] \n\r",i);
dDev++;
}
sWire.endTransmission();
}
if (dDev == 0) printf("find nothing");
while (1);
}
