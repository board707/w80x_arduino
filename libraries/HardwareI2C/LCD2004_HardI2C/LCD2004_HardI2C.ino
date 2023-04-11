#include "Arduino.h"
#include "HardwareI2C.h"
#include "LCD2004_PCF8547.h"

#define LCD_WIDTH   20
#define LCD_HEIGHT  4

uint8_t data[] =
{
  // Pac Close
  0b01110,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b01110,

  // Pack Right
  0b01110,
  0b11111,
  0b11110,
  0b11100,
  0b11100,
  0b11110,
  0b11111,
  0b01110,

  // Ghost 1
  0b01110,
  0b11111,
  0b11111,
  0b10101,
  0b11111,
  0b11111,
  0b11111,
  0b10101,

  // Ghost 2
  0b01110,
  0b11111,
  0b11111,
  0b10101,
  0b11111,
  0b11111,
  0b11111,
  0b01010,

  // Dot
  0b00000,
  0b00000,
  0b00000,
  0b01100,
  0b01100,
  0b00000,
  0b00000,
  0b00000,

  // Maze Left
  0b00000,
  0b01111,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b01111,
  0b00000,

  // Maze Center
  0b00000,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b00000,

  // Maze Right
  0b00000,
  0b11110,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b11110,
  0b00000,
};



HardwareI2C lsw;

void setup()
{

  lsw.begin();

  LCDInit (&lsw);

  delay(500);

  // Set CGRAM Address
  uint8_t offset = 0;
  LCDWriteInstructionByte (0b01000000 | offset);
  delayMicroseconds(40);
  LCDWriteData (&data[0], sizeof(data));

  LCDClear ();
  LCDSetBacklightOn();
}


void loop()
{
  delay (1000);

  LCDWriteDataString (0, 0, (char*)"8 Programmable Chars");
  LCDWriteDataString (1, 2, (char*)"0:");
  LCDWriteDataByte (0);
  LCDWriteDataString (6, 2, (char*)"1:\1  2:\2  3:\3");
  LCDWriteDataString (1, 3, (char*)"4:\4  5:\5  6:\6  7:\7");

  delay (4000);

  LCDWriteDataString (0, 1, (char*)"\5\6\6\6\6\6\6\6\6\6\6\6\6\6\6\6\6\6\6\7");
  LCDWriteDataString (0, 2, (char*)"\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4");
  LCDWriteDataString (0, 3, (char*)"\5\6\6\6\6\6\6\6\6\6\6\6\6\6\6\6\6\6\6\7");

  delay (2000);

  for (int x=0; x<24; x++)
  {
    if (x < 20)
    {
      LCDSetXY (x , 2);
      LCDWriteDataByte (x & 1);
    }

    if (x > 4)
    {
      LCDSetXY (x-4, 2);
      LCDWriteDataByte (2 + (x & 1));
    }

    delay(250);
    
    if (x < 20)
    {
      LCDSetXY (x, 2);
      LCDWriteDataByte (32);
    }

    if (x > 4)
    {
      LCDSetXY (x-4, 2);
      LCDWriteDataByte (32);
    }
  }

  delay(3000);

  LCDClear ();

  int x = 0;
  int y = 0;
  int xm = 1;
  int ym = 1;

  for (int idx=0; idx<40; idx++)
  {
    LCDWriteDataString (x, y, (char*)"Sub-Etha!");
    delay(250);
    LCDWriteDataString (x, y, (char*)"         ");

    x = x + xm;
    if ((x < 1) || (x >= LCD_WIDTH-8-1))
    {
      xm = -xm;
    }

    y = y + ym;
    if ((y < 1) || (y >= LCD_HEIGHT-1))
    {
      ym = -ym;
    }
  }

  delay(3000);
}
