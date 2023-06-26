#ifndef __SDIO_SPI_DRIVER_H__
#define __SDIO_SPI_DRIVER_H__

// Упрощенная версия SDIO -> SPI драйвера.
// Адаптировано AnatolSher из оригинального, предоставленного компанией WinnerMicro
// Цель адаптации - проверка работоспособности.

#include "Arduino.h"

#define SDIO_HOST ((SDIO_TypeDef *)SDIO_BASE)

// Пины для передачи данных SDIO/CLK и SDIO/CMD. См. возможные альтернативы в даташите
// Тестировалось на плате Air103

#define LCD_SCL_PIN		PB6		// SCL
#define LCD_MOSI_PIN	PB7 	// MOSI

void init_sdio_spi_mode()
{
  __HAL_RCC_SDIO_CLK_ENABLE();

  __HAL_AFIO_REMAP_SDIO_CLK(pin_Map[LCD_SCL_PIN].pPort, pin_Map[LCD_SCL_PIN].halPin);  /*CK*/
  __HAL_AFIO_REMAP_SDIO_CMD(pin_Map[LCD_MOSI_PIN].pPort, pin_Map[LCD_MOSI_PIN].halPin);  /*CMD*/

  CLEAR_BIT(RCC->RST, RCC_RST_SDIO); // Программный сброс контроллера SDIO
  SET_BIT(RCC->RST, RCC_RST_SDIO);
  while (READ_BIT(RCC->RST, RCC_RST_SDIO) == 0);
  // Тактирование расчитывается исходя из тактовой частоты процессора.
  // В данном случае cpuclk/2
  wm_sys_clk sysclk;
  SystemClock_Get(&sysclk);
  SDIO_HOST->SEL = 0xC0 | (sysclk.cpuclk / 2 - 1); // enable module, enable mmcclk

  SDIO_HOST->CR = 0x542 | 0 << 3;// auto transfer, mmc mode.
  //SDIO_HOST->CR = 0x542 | (0b111 << 3); // 111 1/16 for debug
  SDIO_HOST->IM = 0x100; // unmask sdio data interrupt.
  SDIO_HOST->CRCCTL = 0x00;
  SDIO_HOST->TIMEOUT = 0;
  SDIO_HOST->BYTECNT = 0;
}

void sdio_spi_put(uint8_t d)  //Отправить один байт
{
  SDIO_HOST->BUFCR = 0x4820;
  SDIO_HOST->DATABUF[0] = d;

  SDIO_HOST->BYTECNT = 1;
  SDIO_HOST->IO = 0x01;
  while (1) {
    if ((SDIO_HOST->IO & 0x01) == 0x00)
      break;
  }
}
void sdio_spi_put16(uint16_t d) //Отправить два байта
{
  SDIO_HOST->BUFCR = 0x4820;
  SDIO_HOST->DATABUF[0] = d;

  SDIO_HOST->BYTECNT = 2;
  SDIO_HOST->IO = 0x01;
  while (1) {
    if ((SDIO_HOST->IO & 0x01) == 0x00)
      break;
  }
}

#endif
