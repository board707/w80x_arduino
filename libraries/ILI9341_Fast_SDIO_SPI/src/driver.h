#ifndef __SDIO_SPI_DRIVER_H__
#define __SDIO_SPI_DRIVER_H__

// Basic SDIO -> SPI driver
// Adapted by AnatolSher from original WinnerMicro code
// for test purposes.

#include "Arduino.h"

#define SDIO_HOST ((SDIO_TypeDef *)SDIO_BASE)

// Data transfer pins SDIO/CLK & SDIO/CMD. See datasheet for alternatives.
// Tested on Air103 board

#define LCD_SCL_PIN		PB6		// SCL
#define LCD_MOSI_PIN	PB7 	// MOSI

void init_sdio_spi_mode()
{
  __HAL_RCC_SDIO_CLK_ENABLE();

  __HAL_AFIO_REMAP_SDIO_CLK(pin_Map[LCD_SCL_PIN].pPort, pin_Map[LCD_SCL_PIN].halPin);  /*CK*/
  __HAL_AFIO_REMAP_SDIO_CMD(pin_Map[LCD_MOSI_PIN].pPort, pin_Map[LCD_MOSI_PIN].halPin);  /*CMD*/

  CLEAR_BIT(RCC->RST, RCC_RST_SDIO); //  SDIO controller reset
  SET_BIT(RCC->RST, RCC_RST_SDIO);
  while (READ_BIT(RCC->RST, RCC_RST_SDIO) == 0);
  // SDIO clock calculated based on cpuclk
  // We use cpuclk/2
  wm_sys_clk sysclk;
  SystemClock_Get(&sysclk);
  SDIO_HOST->SEL = 0xC0 | (sysclk.cpuclk / 2 - 1); // enable module, enable mmcclk

  //SDIO_HOST->CR = 0x542 | 0 << 3;// auto transfer, mmc mode.
  SDIO_HOST->CR = 0x542 | (1 << 3);
  //SDIO_HOST->CR = 0x542 | (0b111 << 3); // 111 1/16 for debug
  SDIO_HOST->IM = 0x100; // unmask sdio data interrupt.
  SDIO_HOST->CRCCTL = 0x00;
  SDIO_HOST->TIMEOUT = 0;
  SDIO_HOST->BYTECNT = 0;
}

void sdio_spi_put(uint8_t d)  // Transfer single byte
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
void sdio_spi_put16(uint16_t d) //Transfer two bytes
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
