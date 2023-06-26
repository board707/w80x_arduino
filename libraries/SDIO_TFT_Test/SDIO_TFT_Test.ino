#include <Arduino.h>
#include "lcd.h"

// Тестировалось на плате Air103 с TFT дисплеем на ili9341 240*320 SPI
// Соединять в следующей последовательности
//    PB9    RS/DC
//    PB10   RST
//    PB11   CS
//    PB6    SCL
//    PB7    SDI
 

uint8_t index = 0;
uint16_t buff[] = { BLACK, RED, GREEN, BLUE, YELLOW, PURPLE, CYAN, MAGENTA, WHITE, GREENYELLOW, VIOLET, AQUA };

void setup() {
  Lcd_Init();
  Lcd_Orientation(scr_normal);
}


void loop() {
    for (index = 0; index < sizeof(buff); index++) {
    Lcd_Clear(buff[index]);
    delay(1500);
  }

}
