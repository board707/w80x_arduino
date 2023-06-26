/*
  Драйвер дисплея на базе ILI9341
  Тестировалось на 2,4" и 3,2" TFT SPI VER1.0 240x320
  с платой Air103
*/
#include "Arduino.h"
#include "driver.h"

// Пины для управления дисплеем. Можно выбирать любые.
#define LCD_DC_PIN		PB9		// RS/DC
#define LCD_RST_PIN		PB10	// RST
#define LCD_CS_PIN		PB11	// CS

// Разрешение дисплея по узкому краю
#define LCD_WIDTH        240
#define LCD_HIGH         320

#define TFT_MADCTL  0x36
#define TFT_MAD_MY  0x80
#define TFT_MAD_MX  0x40
#define TFT_MAD_MV  0x20
#define TFT_MAD_ML  0x10
#define TFT_MAD_BGR 0x00
#define TFT_MAD_MH  0x04
#define TFT_MAD_RGB 0x08

// Базовые цвета - могут не совпасть с ожиданиями :). Зависит от гамма коррекции
#define BLACK                      0x0000
#define BLUE                       0x001F
#define RED                        0xF800
#define GREEN                      0x0400
#define CYAN                       0x07FF
#define MAGENTA                    0xF81F
#define YELLOW                     0xFFE0
#define WHITE                      0xFFFF
#define VIOLET                     0xEC1D
#define AQUA                       0x07FF
#define GREENYELLOW                0x9E66
#define PURPLE                     0x8010

// Ориентация дисплея
typedef enum {
  scr_normal = 0, scr_CW = 1, scr_CCW = 2, scr_180 = 3
} ScrOrientation_TypeDef;

uint16_t _width, _height;	//Ширина,высота после изменения ориентации

void Lcd_Pin_Init(void) {
  pinMode(LCD_DC_PIN, OUTPUT);
  pinMode(LCD_RST_PIN, OUTPUT);
  pinMode(LCD_CS_PIN, OUTPUT);
}

void Lcd_Hard_Reset(void)	// Аппаратный сброс
{
  digitalWrite(LCD_RST_PIN, HIGH);
  delay(5);
  digitalWrite(LCD_RST_PIN, LOW);
  delay(15);
  digitalWrite(LCD_RST_PIN, HIGH);
  delay(15);
}

void Lcd_Set_DC_CS(bool dc, bool cs) {
  if (dc == true)
    digitalWrite(LCD_DC_PIN, HIGH);
  else
    digitalWrite(LCD_DC_PIN, LOW);

  if (cs == true)
    digitalWrite(LCD_CS_PIN, HIGH);
  else
    digitalWrite(LCD_CS_PIN, LOW);
}

void Lcd_Write_Data(uint8_t data) {
  Lcd_Set_DC_CS(true, false);
  sdio_spi_put(data);
  Lcd_Set_DC_CS(true, true);
}

void Lcd_Write_Data16(uint16_t data) {
  Lcd_Set_DC_CS(true, false);
  sdio_spi_put16(data);
  Lcd_Set_DC_CS(true, true);
}

void Lcd_Write_Com(uint8_t cmd) {
  Lcd_Set_DC_CS(false, false);
  sdio_spi_put(cmd);
  Lcd_Set_DC_CS(true, true);
}

void Lcd_Soft_Reset()	//Программный сброс
{
  Lcd_Write_Com(0x01);
  Lcd_Write_Data(0x01);
}

void Lcd_Init(void) {
  init_sdio_spi_mode();
  Lcd_Pin_Init();
  Lcd_Hard_Reset();
  Lcd_Soft_Reset();
  
  Lcd_Write_Com(0xCB);
  Lcd_Write_Data(0x39);
  Lcd_Write_Data(0x2C);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x34);
  Lcd_Write_Data(0x02);

  Lcd_Write_Com(0xCF);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0XC1);
  Lcd_Write_Data(0X30);

  Lcd_Write_Com(0xE8);
  Lcd_Write_Data(0x85);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x78);

  Lcd_Write_Com(0xEA);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x00);

  Lcd_Write_Com(0xED);
  Lcd_Write_Data(0x64);
  Lcd_Write_Data(0x03);
  Lcd_Write_Data(0X12);
  Lcd_Write_Data(0X81);

  Lcd_Write_Com(0xF7);
  Lcd_Write_Data(0x20);

  Lcd_Write_Com(0xC0);    //Power control
  Lcd_Write_Data(0x23);   //VRH[5:0]

  Lcd_Write_Com(0xC1);    //Power control
  Lcd_Write_Data(0x10);   //SAP[2:0];BT[3:0]

  Lcd_Write_Com(0xC5);    //VCM control
  Lcd_Write_Data(0x3e);   //Contrast
  Lcd_Write_Data(0x28);

  Lcd_Write_Com(0xC7);    //VCM control2
  Lcd_Write_Data(0x86);   //--

  Lcd_Write_Com(TFT_MADCTL);    // Memory Access Control
  Lcd_Write_Data(TFT_MAD_MX | TFT_MAD_RGB); // Portrait, RGB

  Lcd_Write_Com(0x3A);
  Lcd_Write_Data(0x55);

  Lcd_Write_Com(0xB1);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x18);

  Lcd_Write_Com(0xB6);    // Display Function Control
  Lcd_Write_Data(0x08);
  Lcd_Write_Data(0x82);
  Lcd_Write_Data(0x27);
  
  Lcd_Write_Com(0xF2);    // 3Gamma Function Disable
  Lcd_Write_Data(0x00);

  Lcd_Write_Com(0x26);    //Gamma curve selected
  Lcd_Write_Data(0x01);

  Lcd_Write_Com(0xE0);    //Set Gamma
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

  Lcd_Write_Com(0XE1);    //Set Gamma
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

  Lcd_Write_Com(0x11);    //Exit Sleep
  delay(120);

  Lcd_Write_Com(0x29);    //Display on
  Lcd_Write_Com(0x2c);
}

void Lcd_Orientation(ScrOrientation_TypeDef orientation) {
  Lcd_Write_Com(TFT_MADCTL); // Memory data access control:
  switch (orientation) {
    case scr_CW:
      Lcd_Write_Data(TFT_MAD_MV | TFT_MAD_RGB);
      _width = LCD_HIGH;
      _height = LCD_WIDTH;

      break;
    case scr_CCW:
      Lcd_Write_Data(TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_MV | TFT_MAD_RGB);
      _width = LCD_HIGH;
      _height = LCD_WIDTH;

      break;
    case scr_180:
      Lcd_Write_Data(TFT_MAD_MY | TFT_MAD_RGB);
      _width = LCD_WIDTH;
      _height = LCD_HIGH;

      break;
    default:
      Lcd_Write_Data(TFT_MAD_MX | TFT_MAD_RGB);
      _width = LCD_WIDTH;
      _height = LCD_HIGH;
      break;
  }
}

void Lcd_Set_Window(uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE) {
  Lcd_Write_Com(0x2a);
  Lcd_Write_Data(XS >> 8);
  Lcd_Write_Data(XS);
  Lcd_Write_Data(XE >> 8);
  Lcd_Write_Data(XE);
  Lcd_Write_Com(0x2b);
  Lcd_Write_Data(YS >> 8);
  Lcd_Write_Data(YS);
  Lcd_Write_Data(YE >> 8);
  Lcd_Write_Data(YE);
  Lcd_Write_Com(0x2c);
  Lcd_Write_Data(0x01);
}

void Lcd_Clear(uint16_t color) {
  uint32_t i;
  Lcd_Set_Window(0, 0, (_width - 1), (_height - 1));
  Lcd_Set_DC_CS(true, false);
  for (i = 0; i < (uint32_t)(_width * _height); i++) {
    sdio_spi_put16(color);
  }
  Lcd_Set_DC_CS(true, true);
}
