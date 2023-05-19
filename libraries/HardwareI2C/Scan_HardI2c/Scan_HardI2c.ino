#include "Arduino.h"
#include "HardwareI2C.h"

HardwareI2C hWire;
uint8_t dDev = 0;
uint8_t mDev[128] = {0};
void setup() {

  hWire.begin();
  printf("begin to scan...\n\r");
    for (unsigned char i = 0; i <= 127; i++) {
    delay(100); // Это обязательно! У HAL есть определенный таймаут для ожидания ACK (255 приращений в цикле ожидания)
    if (hWire.beginTransmission(i)) {
      mDev[dDev]=i;
      dDev++;
    }
    hWire.endTransmission();
  }
  // На вывод в терминал сообщений типа i2c ack error - не обращаем внимание. Его генерирует HAL и подавить его нет возможности.
  if (dDev == 0) printf("find nothing");
  else
  {
    printf("\n\r");
    for(int i=0;i<=127;i++) {
      if (mDev[i] != 0) printf("Found i2c device at address [0x%X] \n\r",mDev[i]);
    }
  }
}

void loop() {
  ;
}
