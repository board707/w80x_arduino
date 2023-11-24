#include "Arduino.h"
#define SDIO_HOST ((SDIO_TypeDef *)SDIO_BASE)

#define LCD_SCL_PIN    PB6   // SCL
#define LCD_MOSI_PIN  PB7   // MOSI

static DMA_HandleTypeDef hdma_sdspi_tx;

void init_sdio_spi_mode()
{
  __HAL_RCC_SDIO_CLK_ENABLE();
  __HAL_RCC_DMA_CLK_ENABLE();
  __HAL_AFIO_REMAP_SDIO_CLK(pin_Map[LCD_SCL_PIN].pPort, pin_Map[LCD_SCL_PIN].halPin);  /*CK*/
  __HAL_AFIO_REMAP_SDIO_CMD(pin_Map[LCD_MOSI_PIN].pPort, pin_Map[LCD_MOSI_PIN].halPin);  /*CMD*/
  
  hdma_sdspi_tx.Instance = DMA_Channel0;
  hdma_sdspi_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_sdspi_tx.Init.DestInc = DMA_DINC_DISABLE;
  hdma_sdspi_tx.Init.SrcInc = DMA_SINC_ENABLE;
  hdma_sdspi_tx.Init.DataAlignment = DMA_DATAALIGN_WORD;
  hdma_sdspi_tx.Init.Mode = DMA_MODE_NORMAL_SINGLE;
  hdma_sdspi_tx.Init.RequestSourceSel = DMA_REQUEST_SOURCE_SDIO;

  HAL_DMA_Init(&hdma_sdspi_tx);

  CLEAR_BIT(RCC->RST, RCC_RST_SDIO); // Программный сброс контроллера SDIO
  SET_BIT(RCC->RST, RCC_RST_SDIO);
  while (READ_BIT(RCC->RST, RCC_RST_SDIO) == 0);
  // Тактирование расчитывается исходя из тактовой частоты процессора.
  // В данном случае cpuclk/2
  wm_sys_clk sysclk;
  SystemClock_Get(&sysclk);
  SDIO_HOST->SEL = 0xC0 | (sysclk.cpuclk / 2 - 1); // enable module, enable mmcclk
  SDIO_HOST->CR = 0x542 | 1 << 3;// auto transfer, mmc mode.
  //SDIO_HOST->CR = 0x542 | (0b111 << 3); // 111 1/16 for debug
  SDIO_HOST->IM = 0x100; // unmask sdio data interrupt.
  SDIO_HOST->CRCCTL = 0x00;
  SDIO_HOST->TIMEOUT = 0;
  SDIO_HOST->BYTECNT = 0;
}

void sdio_spi_put(uint8_t d)  //Transfer one byte
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

void write_sdio_spi_dma(uint32_t* data, uint32_t len) 
{
    while(1){
        if ((SDIO_HOST->IO & 0x01) == 0x00)
            break;
    }
    uint32_t offset=0;
    while(len>0){
        int datalen=len;
        if(len>0xfffc)
            datalen=0xfffc;
        len-=datalen;
        SDIO_HOST->BUFCR = 0x4000; //disable dma,
        HAL_DMA_Start(&hdma_sdspi_tx, (uint32_t) (data+offset), (uint32_t)(SDIO_HOST->DATABUF), datalen);
        SDIO_HOST->BUFCR = 0xC20; //enable dma, write 
        SDIO_HOST->IF |= 0x7ff; // clear all firstly
        SDIO_HOST->BYTECNT = datalen;
        SDIO_HOST->IO = 0x01;
        offset+=datalen/4;
        HAL_DMA_PollForTransfer(&hdma_sdspi_tx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
        while(1){
            if ((SDIO_HOST->BUFCR & 0x400) == 0x00) // Ожидание завершения передачи блока
                break;
        }
    }
}
