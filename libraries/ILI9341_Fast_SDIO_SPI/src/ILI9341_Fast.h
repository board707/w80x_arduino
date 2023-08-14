// Fast ILI9341 240x320 2.2" display library
// (c) 2019-20 by Pawel A. Hernik

/*
 ILI9341 240x320 2.2" LCD pinout (header at the top, from left):
 #1 MISO  -> NC
 #2 LED   -> 3.3V
 #3 SCK   -> SCL/D13/PA5
 #4 SDI   -> MOSI/D11/PA7
 #5 DC    -> D8/PA1  or any digital
 #6 RESET -> D9/PA0  or any digital
 #7 CS    -> D10/PA2 or any digital
 #8 GND   -> GND
 #9 VCC   -> 3.3V
*/

#ifndef _ILI9341_FAST_H_
#define _ILI9341_FAST_H_

// ------------------------------
// remove "define COMPATIBILITY_MODE" for best performance on 16MHz AVR Arduinos
// if defined - the library should work on all Arduino compatible boards
//#define COMPATIBILITY_MODE

// define when CS pin is always connected to the ground
//#define CS_ALWAYS_LOW
// ------------------------------

#include "Arduino.h"
#include "Print.h"
#include <Adafruit_GFX.h>
#include <pgmspace.h>

#define ILI9341_TFTWIDTH  240
#define ILI9341_TFTHEIGHT 320

// Color definitions

#define RGBto565(r,g,b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3)) 
#define RGBIto565(r,g,b,i) ((((((r)*(i))/255) & 0xF8) << 8) | ((((g)*(i)/255) & 0xFC) << 3) | ((((b)*(i)/255) & 0xFC) >> 3)) 

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define	GREY  RGBto565(128,128,128)
#define	LGREY RGBto565(160,160,160)
#define	DGREY RGBto565( 80, 80, 80)

#define	LBLUE RGBto565(100,100,255)
#define	DBLUE RGBto565(  0,  0,128)

class ILI9341 : public Adafruit_GFX {

 public:
  ILI9341(int8_t DC, int8_t RST, int8_t CS = -1);

  void init();
  void begin() { init(); }
  void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void pushColor(uint16_t color);
  void fillScreen(uint16_t color=BLACK);
  void clearScreen() { fillScreen(BLACK); }
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void drawImage(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *img);
  void drawImageF(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *img16);
  void drawImageF(int16_t x, int16_t y, const uint16_t *img16) { drawImageF(x,y,pgm_read_word(img16),pgm_read_word(img16+1),img16+3); } 
  void setRotation(uint8_t r);
  void invertDisplay(boolean mode);
  void partialDisplay(boolean mode);
  void sleepDisplay(boolean mode);
  void enableDisplay(boolean mode);
  void idleDisplay(boolean mode);
  void resetDisplay();
  void setScrollArea(uint16_t tfa, uint16_t bfa);
  void setScroll(uint16_t vsp);
  void setPartArea(uint16_t sr, uint16_t er);

  uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);
  uint16_t color565(uint8_t r, uint8_t g, uint8_t b) { return Color565(r, g, b); } 
  void rgbWheel(int idx, uint8_t *_r, uint8_t *_g, uint8_t *_b);
  uint16_t rgbWheel(int idx);

 protected:
  void displayInit(const uint8_t *addr);
  void writeSPI(uint8_t);
  void writeMulti(uint16_t color, uint16_t num);
  void writeCmd(uint8_t c);
  void writeData(uint8_t d8);
  void writeData16(uint16_t d16);
  void copyMulti(uint8_t *img, uint16_t num);



 private:
  int8_t  csPin, dcPin, rstPin;
};

#endif
