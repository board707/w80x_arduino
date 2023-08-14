#include <Arduino.h>
#include "lcd.h"

// Basic SDIO -> SPI driver example
// 
// The code just fill the display with solid colors using the custom LCD driver
// Tested on Air103 board with ILI9341 240*320 SPI TFT

/* ILI9341 240x320 3.2" V1.0 SPI LCD pinout (header at the top, from left):
 #1 MISO  -> NC
 #2 LED   -> 3.3V (5v)
 #3 SCK   -> PB6
 #4 SDI   -> PB7
 #5 DC    -> PB9
 #6 RESET -> PB10
 #7 CS    -> PB14
 #8 GND   -> GND
 #9 VCC   -> 3.3V (5v)
*/
 

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
