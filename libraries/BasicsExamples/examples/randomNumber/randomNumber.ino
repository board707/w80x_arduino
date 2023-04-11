#include "Arduino.h"

void setup() {
  trngInit();
  randomSeed(rngGet());
}


void loop() {
  printf("true random number   is:%d \n\r",rngGet());
  printf("stdlib random number is:%d \n\r",random(0xFFFF));
}
