#include "Arduino.h"

void setup() {
  Serial.begin(115200);
  trngInit();
  randomSeed(rngGet());
}


void loop() {
  Serial.printf("true random number   is:%d \n\r",rngGet());
  Serial.printf("stdlib random number is:%d \n\r",random(0xFFFF));
}
