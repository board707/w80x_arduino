// Fast ILI9341 240x320 2.2" display library
// (c) 2019-20 by Pawel A. Hernik

#include "ILI9341_Fast.h"
#include <limits.h>
#include "pins_arduino.h"
#include "wiring_private.h"
#include <driver.h>
#include "Arduino.h"

#define DC_DATA     digitalWrite(dcPin, HIGH)
#define DC_COMMAND  digitalWrite(dcPin, LOW)
#define CS_IDLE     digitalWrite(csPin, HIGH)
#define CS_ACTIVE   digitalWrite(csPin, LOW)

// -----------------------------------------
// ILI9341 commands
#define ILI9341_NOP        0x00
#define ILI9341_SWRESET    0x01
#define ILI9341_RDDID      0x04
#define ILI9341_RDDST      0x09
#define ILI9341_RDMODE     0x0A
#define ILI9341_RDMADCTL   0x0B
#define ILI9341_RDPIXFMT   0x0C
#define ILI9341_RDIMGFMT   0x0A
#define ILI9341_RDSELFDIAG 0x0F

#define ILI9341_SLPIN      0x10
#define ILI9341_SLPOUT     0x11
#define ILI9341_PTLON      0x12
#define ILI9341_NORON      0x13

#define ILI9341_INVOFF     0x20
#define ILI9341_INVON      0x21
#define ILI9341_GAMMASET   0x26
#define ILI9341_DISPOFF    0x28
#define ILI9341_DISPON     0x29
#define ILI9341_CASET      0x2A
#define ILI9341_PASET      0x2B
#define ILI9341_RAMWR      0x2C
#define ILI9341_RAMRD      0x2E

#define ILI9341_PTLAR      0x30
#define ILI9341_VSCRDEF    0x33
#define ILI9341_MADCTL     0x36
#define ILI9341_VSCRSADD   0x37
#define ILI9341_PIXFMT     0x3A

#define ILI9341_FRMCTR1    0xB1
#define ILI9341_FRMCTR2    0xB2
#define ILI9341_FRMCTR3    0xB3
#define ILI9341_INVCTR     0xB4
#define ILI9341_DFUNCTR    0xB6

#define ILI9341_PWCTR1     0xC0
#define ILI9341_PWCTR2     0xC1
#define ILI9341_PWCTR3     0xC2
#define ILI9341_PWCTR4     0xC3
#define ILI9341_PWCTR5     0xC4
#define ILI9341_VMCTR1     0xC5
#define ILI9341_VMCTR2     0xC7
#define ILI9341_PWCTRA     0xCB
#define ILI9341_CMD_CF     0xCF

#define ILI9341_RDID1      0xDA
#define ILI9341_RDID2      0xDB
#define ILI9341_RDID3      0xDC
#define ILI9341_RDID4      0xDD

#define ILI9341_GMCTRP1    0xE0
#define ILI9341_GMCTRN1    0xE1
#define ILI9341_TIMCTRA    0xE8
#define ILI9341_TIMCTRB    0xE9
#define ILI9341_TIMCTRC    0xEA
#define ILI9341_POWSEQ     0xED
#define ILI9341_CMD_EF     0xEF

#define ILI9341_EN3GAM     0xF2
#define ILI9341_PUMPRAT    0xF7

#define CMD_DELAY   0x80

#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04

static const uint8_t ILI9341_commands[] PROGMEM = {
  22,  // number of commands
  ILI9341_SWRESET, CMD_DELAY, 5,    // 1
  ILI9341_CMD_EF, 3,      // 2
    0x03, 0x80, 0x02,
  ILI9341_CMD_CF, 3,      // 3
    0x00, 0xC1, 0x30,
  ILI9341_POWSEQ, 4,      // 4
    0x64, 0x03, 0x12, 0x81,
  ILI9341_TIMCTRA, 3,     // 5
    0x85, 0x00, 0x78,
  ILI9341_PWCTRA, 5,      // 6
    0x39, 0x2C, 0x00, 0x34, 0x02,
  ILI9341_PUMPRAT, 1,     // 7
    0x20,
  ILI9341_TIMCTRC, 2,     // 8
    0x00, 0x00,
  ILI9341_PWCTR1, 1,    // 9 power control 
    0x23,       // VRH[5:0] 
  ILI9341_PWCTR2, 1,    // 10 power control 
    0x10,       // SAP[2:0];BT[3:0]  
  ILI9341_VMCTR1, 2,    // 11 VCM control 
    0x3e, 0x28,
  ILI9341_VMCTR2, 1,    // 12 VCM control2 
    0x86,       // --
  ILI9341_MADCTL, 1,    // 13
    (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR),
  ILI9341_PIXFMT, 1,    // 14
    0x55,
  ILI9341_FRMCTR1, 2,   // 15
    0x00, 0x18,
  ILI9341_DFUNCTR, 3,   // 16
    0x08, 0x82, 0x27,
  ILI9341_EN3GAM, 1,      // 17 3Gamma Function Disable 
    0x00,
  ILI9341_GAMMASET, 1,    // 18 Gamma curve selected 
    0x01,
  ILI9341_GMCTRP1, 15,    // 19 Set Gamma 
    0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
  ILI9341_GMCTRN1, 15,    // 20
    0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
  ILI9341_SLPOUT, CMD_DELAY, 120, // 21
  ILI9341_DISPON, 0,    // 22
};
// -----------------------------------------
  

// ----------------------------------------------------------
// speed test results:
// in AVR best performance mode -> about 7.1 Mbps
// in compatibility mode (SPI.transfer(c)) -> about 4 Mbps

inline void ILI9341::writeSPI(uint8_t c) 
{
    sdio_spi_put(c);
}

// ----------------------------------------------------------
// fast method to send multiple 16-bit values via SPI
inline void ILI9341::writeMulti(uint16_t color, uint16_t num)
{
  while(num--) { sdio_spi_put(color>>8);  sdio_spi_put(color); }
} 
// ----------------------------------------------------------
// fast method to send multiple 16-bit values from RAM via SPI
inline void ILI9341::copyMulti(uint8_t *img, uint16_t num)
{
  while(num--) { sdio_spi_put(*(img+1)); sdio_spi_put(*(img+0)); img+=2; }
} 
// ----------------------------------------------------------
ILI9341::ILI9341(int8_t dc, int8_t rst, int8_t cs) : Adafruit_GFX(ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT) 
{
  csPin = cs;
  dcPin = dc;
  rstPin = rst;
}


// ----------------------------------------------------------
void ILI9341::init() 
{
  pinMode(dcPin, OUTPUT);
  pinMode(csPin, OUTPUT);
  pinMode(rstPin, OUTPUT);

  init_sdio_spi_mode();

  if(rstPin != -1) {
    pinMode(rstPin, OUTPUT);
    digitalWrite(rstPin, HIGH);
    delay(5);
    digitalWrite(rstPin, LOW);
    delay(20);
    digitalWrite(rstPin, HIGH);
    delay(150);
  }

  _width  = ILI9341_TFTWIDTH;
  _height = ILI9341_TFTHEIGHT;
  displayInit(ILI9341_commands);
  setRotation(2);
}

// ----------------------------------------------------------
void ILI9341::writeCmd(uint8_t c) 
{
	  DC_COMMAND;
	  CS_ACTIVE;
sdio_spi_put(c);
CS_IDLE;

}

// ----------------------------------------------------------
void ILI9341::writeData(uint8_t d8) 
{
	  DC_DATA;
	  CS_ACTIVE;
	sdio_spi_put(d8);
	  CS_IDLE;
}

// ----------------------------------------------------------
void ILI9341::writeData16(uint16_t d16) 
{
	  DC_DATA;
	  CS_ACTIVE;
    
	writeMulti(d16,1);

	 CS_IDLE;

}

// ----------------------------------------------------------
void ILI9341::displayInit(const uint8_t *addr) 
{
  uint8_t  numCommands, numArgs;
  uint16_t ms;
  numCommands = pgm_read_byte(addr++);
  while(numCommands--) {
    writeCmd(pgm_read_byte(addr++));
    numArgs  = pgm_read_byte(addr++);
    ms       = numArgs & CMD_DELAY;
    numArgs &= ~CMD_DELAY;
    while(numArgs--) writeData(pgm_read_byte(addr++));

    if(ms) {
      ms = pgm_read_byte(addr++);
      if(ms==255) ms=500;
      delay(ms);
    }
  }
}

// ----------------------------------------------------------
void ILI9341::setRotation(uint8_t m) 
{
  writeCmd(ILI9341_MADCTL);
  rotation = m & 3;
  switch (rotation) {
   case 0:
     writeData(ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
     _width  = ILI9341_TFTWIDTH;
     _height = ILI9341_TFTHEIGHT;
     break;
   case 1:
     writeData(ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
     _width  = ILI9341_TFTHEIGHT;
     _height = ILI9341_TFTWIDTH;
     break;
  case 2:
    writeData(ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
     _width  = ILI9341_TFTWIDTH;
     _height = ILI9341_TFTHEIGHT;
    break;
   case 3:
     writeData(ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
     _width  = ILI9341_TFTHEIGHT;
     _height = ILI9341_TFTWIDTH;
     break;
  }
}

// ----------------------------------------------------------
void ILI9341::setAddrWindow(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye) 
{
	 CS_ACTIVE;
	  DC_COMMAND; writeSPI(ILI9341_CASET);
	  DC_DATA;    writeMulti(xs,1); writeMulti(xe,1);

	  DC_COMMAND; writeSPI(ILI9341_PASET);
	  DC_DATA;    writeMulti(ys,1); writeMulti(ye,1);

	  DC_COMMAND; writeSPI(ILI9341_RAMWR);

	  DC_DATA;
}

// ----------------------------------------------------------
void ILI9341::pushColor(uint16_t color) 
{
	  CS_ACTIVE;

	  writeSPI(color>>8); writeSPI(color);

	  CS_IDLE;
}

// ----------------------------------------------------------
void ILI9341::drawPixel(int16_t x, int16_t y, uint16_t color) 
{
  if(x<0 ||x>=_width || y<0 || y>=_height) return;
  setAddrWindow(x,y,x+1,y+1);
  writeSPI(color>>8); writeSPI(color);

  CS_IDLE;
}

// ----------------------------------------------------------
void ILI9341::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) 
{
  if(x>=_width || y>=_height || h<=0) return;
  if(y+h-1>=_height) h=_height-y;
  setAddrWindow(x, y, x, y+h-1);

  writeMulti(color,h);
  CS_IDLE;
}

// ----------------------------------------------------------
void ILI9341::drawFastHLine(int16_t x, int16_t y, int16_t w,  uint16_t color) 
{
  if(x>=_width || y>=_height || w<=0) return;
  if(x+w-1>=_width)  w=_width-x;
  setAddrWindow(x, y, x+w-1, y);

  writeMulti(color,w);
  CS_IDLE;
}

// ----------------------------------------------------------
void ILI9341::fillScreen(uint16_t color) 
{
  fillRect(0, 0,  _width, _height, color);
}

// ----------------------------------------------------------
void ILI9341::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
  if(x>=_width || y>=_height || w<=0 || h<=0) return;
  if(x+w-1>=_width)  w=_width -x;
  if(y+h-1>=_height) h=_height-y;
  setAddrWindow(x, y, x+w-1, y+h-1);

  uint32_t num = (uint32_t)w*h;
  if(num>0xffff) {
    writeMulti(color,0xffff);
    writeMulti(color,num-0xffff);
  } else writeMulti(color,num);
  CS_IDLE;

}

// ----------------------------------------------------------
// draws image from RAM
void ILI9341::drawImage(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *img16) 
{
  // all protections should be on the application side
  //if(x>=_width || y>=_height || w<=0 || h<=0) return;
  //if(x+w-1>=_width)  w=_width -x;
  //if(y+h-1>=_height) h=_height-y;
  setAddrWindow(x, y, x+w-1, y+h-1);

  copyMulti((uint8_t *)img16, w*h);
  CS_IDLE;
}

// ----------------------------------------------------------
// draws image from flash (PROGMEM)
void ILI9341::drawImageF(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *img16) 
{
  if(x>=_width || y>=_height || w<=0 || h<=0) return;
  setAddrWindow(x, y, x+w-1, y+h-1);

  uint32_t num = (uint32_t)w*h;
  uint16_t num16 = num>>3;
  uint8_t *img = (uint8_t *)img16;
  while(num16--) {
    writeSPI(pgm_read_byte(img+1)); writeSPI(pgm_read_byte(img+0)); img+=2;
    writeSPI(pgm_read_byte(img+1)); writeSPI(pgm_read_byte(img+0)); img+=2;
    writeSPI(pgm_read_byte(img+1)); writeSPI(pgm_read_byte(img+0)); img+=2;
    writeSPI(pgm_read_byte(img+1)); writeSPI(pgm_read_byte(img+0)); img+=2;
    writeSPI(pgm_read_byte(img+1)); writeSPI(pgm_read_byte(img+0)); img+=2;
    writeSPI(pgm_read_byte(img+1)); writeSPI(pgm_read_byte(img+0)); img+=2;
    writeSPI(pgm_read_byte(img+1)); writeSPI(pgm_read_byte(img+0)); img+=2;
    writeSPI(pgm_read_byte(img+1)); writeSPI(pgm_read_byte(img+0)); img+=2;
  }
  uint8_t num8 = num & 0x7;
  while(num8--) { writeSPI(pgm_read_byte(img+1)); writeSPI(pgm_read_byte(img+0)); img+=2; }
  CS_IDLE;

}

// ----------------------------------------------------------
// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t ILI9341::Color565(uint8_t r, uint8_t g, uint8_t b) 
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// ----------------------------------------------------------
void ILI9341::invertDisplay(boolean mode) 
{
  writeCmd(mode ? ILI9341_INVON : ILI9341_INVOFF);
}

// ----------------------------------------------------------
void ILI9341::partialDisplay(boolean mode) 
{
  writeCmd(mode ? ILI9341_PTLON : ILI9341_NORON);
}

// ----------------------------------------------------------
void ILI9341::sleepDisplay(boolean mode) 
{
  writeCmd(mode ? ILI9341_SLPIN : ILI9341_SLPOUT);
  delay(5);
}

// ----------------------------------------------------------
void ILI9341::enableDisplay(boolean mode) 
{
  writeCmd(mode ? ILI9341_DISPON : ILI9341_DISPOFF);
}

// ----------------------------------------------------------
void ILI9341::resetDisplay() 
{
  writeCmd(ILI9341_SWRESET);
  delay(5);
}

// ----------------------------------------------------------
void ILI9341::setScrollArea(uint16_t tfa, uint16_t bfa) 
{
  uint16_t vsa = 320-tfa-bfa;
  writeCmd(ILI9341_VSCRDEF);
  writeData16(tfa);
  writeData16(vsa);
  writeData16(bfa);
}

// ----------------------------------------------------------
void ILI9341::setScroll(uint16_t vsp) 
{
  writeCmd(ILI9341_VSCRSADD);
  writeData16(vsp);
}

// ----------------------------------------------------------
void ILI9341::setPartArea(uint16_t sr, uint16_t er) 
{
  writeCmd(ILI9341_PTLAR);
  writeData16(sr);
  writeData16(er);
}

// ------------------------------------------------
// Input a value 0 to 511 (85*6) to get a color value.
// The colours are a transition R - Y - G - C - B - M - R.
void ILI9341::rgbWheel(int idx, uint8_t *_r, uint8_t *_g, uint8_t *_b)
{
  idx &= 0x1ff;
  if(idx < 85) { // R->Y  
    *_r = 255; *_g = idx * 3; *_b = 0;
    return;
  } else if(idx < 85*2) { // Y->G
    idx -= 85*1;
    *_r = 255 - idx * 3; *_g = 255; *_b = 0;
    return;
  } else if(idx < 85*3) { // G->C
    idx -= 85*2;
    *_r = 0; *_g = 255; *_b = idx * 3;
    return;  
  } else if(idx < 85*4) { // C->B
    idx -= 85*3;
    *_r = 0; *_g = 255 - idx * 3; *_b = 255;
    return;    
  } else if(idx < 85*5) { // B->M
    idx -= 85*4;
    *_r = idx * 3; *_g = 0; *_b = 255;
    return;    
  } else { // M->R
    idx -= 85*5;
    *_r = 255; *_g = 0; *_b = 255 - idx * 3;
   return;
  }
} 

uint16_t ILI9341::rgbWheel(int idx)
{
  uint8_t r,g,b;
  rgbWheel(idx, &r,&g,&b);
  return RGBto565(r,g,b);
}

// ------------------------------------------------
