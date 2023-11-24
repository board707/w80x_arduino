#include "Arduino.h"
#include "driver.h"
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

#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04

#define LCD_WIDTH        320
#define LCD_HEIGHT       240

uint16_t _width, _height;  //Ширина,высота после изменения ориентации
uint8_t dc_pin,rst_pin;     // CS пин заземлен

void Lcd_Hard_Reset(void) // Аппаратный сброс
{
  digitalWrite(rst_pin, HIGH);
  delay(5);
  digitalWrite(rst_pin, LOW);
  delay(15);
  digitalWrite(rst_pin, HIGH);
  delay(15);
}

void Lcd_Set_DC_CS(bool dc, bool cs) {
  if (dc == true)  digitalWrite(dc_pin, HIGH);
  else  digitalWrite(dc_pin, LOW);
}

void Lcd_Write_Data(uint8_t data) {
  Lcd_Set_DC_CS(true, false);
  sdio_spi_put(data);
  Lcd_Set_DC_CS(true, true);
}

void Lcd_Write_Com(uint8_t cmd) {
  Lcd_Set_DC_CS(false, false);
  sdio_spi_put(cmd);
  Lcd_Set_DC_CS(true, true);
}

void Lcd_Soft_Reset() //Программный сброс
{
  Lcd_Write_Com(ILI9341_SWRESET);
  Lcd_Write_Data(0x01);
}
void displayInit() 
{
  Lcd_Hard_Reset();
  Lcd_Soft_Reset();
  delay(100);
  Lcd_Write_Com(ILI9341_PWCTRA);
  Lcd_Write_Data(0x39);
  Lcd_Write_Data(0x2C);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x34);
  Lcd_Write_Data(0x02);
  delay(10);
  Lcd_Write_Com(ILI9341_CMD_CF);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0XC1);
  Lcd_Write_Data(0X30);
  delay(10);
  Lcd_Write_Com(ILI9341_TIMCTRA);
  Lcd_Write_Data(0x85);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x78);
  delay(10);
  Lcd_Write_Com(ILI9341_TIMCTRC);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x00);
  delay(10);
  Lcd_Write_Com(ILI9341_POWSEQ);
  Lcd_Write_Data(0x64);
  Lcd_Write_Data(0x03);
  Lcd_Write_Data(0X12);
  Lcd_Write_Data(0X81);
  delay(10);
  Lcd_Write_Com(ILI9341_PUMPRAT);
  Lcd_Write_Data(0x20);
  delay(10);
  Lcd_Write_Com(ILI9341_PWCTR1);    //Power control
  Lcd_Write_Data(0x23);   //VRH[5:0]
  delay(10);
  Lcd_Write_Com(ILI9341_PWCTR2);    //Power control
  Lcd_Write_Data(0x10);   //SAP[2:0];BT[3:0]
  delay(10);
  Lcd_Write_Com(ILI9341_VMCTR1);    //VCM control
  Lcd_Write_Data(0x3e);   //Contrast
  Lcd_Write_Data(0x28);
  delay(10);
  Lcd_Write_Com(ILI9341_VMCTR2);    //VCM control2
  Lcd_Write_Data(0x86);   //--
  delay(10);
  Lcd_Write_Com(ILI9341_MADCTL);    // Memory Access Control
  Lcd_Write_Data(ILI9341_MADCTL_MX | ILI9341_MADCTL_RGB); // Portrait, RGB
  delay(10);
  Lcd_Write_Com(ILI9341_PIXFMT);
  Lcd_Write_Data(0x55);
  delay(10);
  Lcd_Write_Com(ILI9341_FRMCTR1);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x18);
  delay(10);
  Lcd_Write_Com(ILI9341_DFUNCTR);    // Display Function Control
  Lcd_Write_Data(0x08);
  Lcd_Write_Data(0x82);
  Lcd_Write_Data(0x27);
  delay(10);
  Lcd_Write_Com(ILI9341_EN3GAM);    // 3Gamma Function Disable
  Lcd_Write_Data(0x00);
  delay(10);
  Lcd_Write_Com(ILI9341_GAMMASET);    //Gamma curve selected
  Lcd_Write_Data(0x01);
  delay(10);
  Lcd_Write_Com(ILI9341_GMCTRP1);    //Set Gamma
  Lcd_Write_Data(0x0F);
  Lcd_Write_Data(0x31);
  Lcd_Write_Data(0x2B);
  Lcd_Write_Data(0x0C);
  Lcd_Write_Data(0x0E);
  Lcd_Write_Data(0x08);
  Lcd_Write_Data(0x4E);
  Lcd_Write_Data(0xF1);
  Lcd_Write_Data(0x37);
  Lcd_Write_Data(0x07);
  Lcd_Write_Data(0x10);
  Lcd_Write_Data(0x03);
  Lcd_Write_Data(0x0E);
  Lcd_Write_Data(0x09);
  Lcd_Write_Data(0x00);
  delay(10);
  Lcd_Write_Com(ILI9341_GMCTRN1);    //Set Gamma
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x0E);
  Lcd_Write_Data(0x14);
  Lcd_Write_Data(0x03);
  Lcd_Write_Data(0x11);
  Lcd_Write_Data(0x07);
  Lcd_Write_Data(0x31);
  Lcd_Write_Data(0xC1);
  Lcd_Write_Data(0x48);
  Lcd_Write_Data(0x08);
  Lcd_Write_Data(0x0F);
  Lcd_Write_Data(0x0C);
  Lcd_Write_Data(0x31);
  Lcd_Write_Data(0x36);
  Lcd_Write_Data(0x0F);
  delay(10);
  Lcd_Write_Com(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  Lcd_Write_Com(ILI9341_DISPON);    //Display on
  Lcd_Write_Com(ILI9341_RAMWR);

}

void Lcd_Init(uint8_t dc, uint8_t rst) {
  pinMode(dc, OUTPUT); dc_pin = dc;
  pinMode(rst, OUTPUT); rst_pin = rst;
  init_sdio_spi_mode();
  Lcd_Hard_Reset();
  Lcd_Soft_Reset();
  displayInit();
}
void Lcd_Orientation(uint8_t m) 
{
  Lcd_Write_Com(ILI9341_MADCTL);
  uint8_t rotation = m & 3;
  switch (rotation) {
   case 0:
     Lcd_Write_Data(ILI9341_MADCTL_MX | ILI9341_MADCTL_RGB);
     _width  = LCD_WIDTH;
     _height = LCD_HEIGHT;
     break;
   case 1:
     Lcd_Write_Data(ILI9341_MADCTL_MV | ILI9341_MADCTL_RGB);
     _width  = LCD_HEIGHT;
     _height = LCD_WIDTH;
     break;
  case 2:
    Lcd_Write_Data(ILI9341_MADCTL_MY | ILI9341_MADCTL_RGB);
     _width  = LCD_WIDTH;
     _height = LCD_HEIGHT;
    break;
   case 3:
     Lcd_Write_Data(ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_RGB);
     _width  = LCD_HEIGHT;
     _height = LCD_WIDTH;
     break;
  }
}

void Lcd_Set_Window(uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE) {
  Lcd_Write_Com(ILI9341_CASET);Lcd_Write_Data(XS >> 8);Lcd_Write_Data(XS);Lcd_Write_Data(XE >> 8);Lcd_Write_Data(XE);
  Lcd_Write_Com(ILI9341_PASET);Lcd_Write_Data(YS >> 8);Lcd_Write_Data(YS);Lcd_Write_Data(YE >> 8);Lcd_Write_Data(YE);
  Lcd_Write_Com(ILI9341_RAMWR);  Lcd_Write_Data(0x01);
}

void Lcd_Draw_Rect(unsigned short x, unsigned short y, unsigned short width, unsigned short height, uint16_t *data)
{
    Lcd_Set_Window(x, y, x + width - 1, y + height - 1);
    write_sdio_spi_dma((uint32_t *)data, width * height*2);
}
