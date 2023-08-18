// W80x SPI & DMA libraries example
//
// Transfer data buffer via SPI asynchonically
//
// Dmitry Dmitriev (c) 2023


#include <SPI.h>
#include <W_DMA.h>

// Data array to be send
#define DATASIZE 128
uint8_t pData[DATASIZE] = {0};

void setup() {
  SPI.begin();
  Serial.begin(115200);

  // get hardware SPI handle, will used for setup dma transfer
  SPI_HandleTypeDef *hspi =  SPI.getHalhandle();

  // populate the data array
  for (int i = 0; i < DATASIZE; i++ ) {
    pData[i] = i;
  }

  //setup dma
  uint8_t dma_ch = claim_unused_dma_channel();
  if (dma_ch != -1) {    // dma channel available?

    // configure dma channel to be used in SPI_TX direction transfer
    DMA_HandleTypeDef* hdma = dma_simple_config(dma_ch, DMA_REQUEST_SOURCE_SPI_TX);

    // link dma channel with hspi handle
    __HAL_LINKDMA(hspi, hdmatx, *hdma);
    
    // init preconfigured dma channel
    dma_init(hdma);
  }

  //enable SPI interrupt
  HAL_NVIC_SetPriority(SPI_LS_IRQn, 1);
  HAL_NVIC_EnableIRQ(SPI_LS_IRQn);

  // transfer pData array via SPI with dma
  uint32_t res = HAL_SPI_Transmit_DMA(hspi, pData, sizeof(pData));

  if (res == 0) Serial.printf("Data transferred successfully!\n");
  else Serial.printf("Tranfer failed, HAL error code = %d\n", res);
  
  delay(200);

  // next....
  /*
  res = HAL_SPI_Transmit_DMA(hspi, pData, sizeof(pData));
  Serial.printf("HAL error code = %d\n", res);
  */
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
}
