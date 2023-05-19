#include "Arduino.h"
/*
* This code illustrates using of true random numeric generator(TRNG), 
* part of w80x inboard cryptomodule
*
* Std lib random functions also supported
*
*/
void setup() {
  Serial.begin(115200);
  trngInit();            // TRNG init
  randomSeed(rngGet());
}


void loop() {
  Serial.printf("true random number   is:%d \n\r",rngGet());
  Serial.printf("stdlib random number is:%d \n\r",random(0xFFFF));
}
