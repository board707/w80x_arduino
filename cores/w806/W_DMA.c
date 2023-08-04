#include "W_DMA.h"

// array to control using of DMA channels
bool used_dma_channels[8] = {0};

DMA_Channel_TypeDef* DMA_CH[8] = {DMA_Channel0, DMA_Channel1, DMA_Channel2, DMA_Channel3,
                                DMA_Channel4, DMA_Channel5, DMA_Channel6, DMA_Channel7};

DMA_HandleTypeDef hdma_channel[8];

int8_t claim_unused_dma_chan() {
  int i = 0;
  while (i < 8) {
    if (!used_dma_channels[i] )  {used_dma_channels[i] = true; return i;}
    i++; 
  }    
  return -1;
}

DMA_HandleTypeDef*  dma_simple_config(uint8_t ch, uint32_t request_src) {

     
        hdma_channel[ch].Instance = DMA_CH[ch];

     switch (request_src) {
        case DMA_REQUEST_SOURCE_SPI_RX:
        case DMA_REQUEST_SOURCE_ADC_CH0:
        case DMA_REQUEST_SOURCE_ADC_CH1: 
        case DMA_REQUEST_SOURCE_ADC_CH2: 
        case DMA_REQUEST_SOURCE_ADC_CH3:      
        case DMA_REQUEST_SOURCE_I2S_RX:   
           hdma_channel[ch].Init.Direction = DMA_PERIPH_TO_MEMORY;
           hdma_channel[ch].Init.DestInc = DMA_DINC_ENABLE;
           hdma_channel[ch].Init.SrcInc = DMA_SINC_DISABLE;
        break;

        case DMA_REQUEST_SOURCE_SPI_TX:
        case DMA_REQUEST_SOURCE_I2S_TX:
           hdma_channel[ch].Init.Direction = DMA_MEMORY_TO_PERIPH;
           hdma_channel[ch].Init.DestInc = DMA_DINC_DISABLE;
           hdma_channel[ch].Init.SrcInc = DMA_SINC_ENABLE;
           break;
        }
       
       hdma_channel[ch].Init.DataAlignment = DMA_DATAALIGN_WORD;
       hdma_channel[ch].Init.Mode = DMA_MODE_NORMAL_SINGLE;
       hdma_channel[ch].Init.RequestSourceSel = request_src;

       return &(hdma_channel[ch]);
}

uint8_t dma_init(DMA_HandleTypeDef* hdma) {

     __HAL_RCC_DMA_CLK_ENABLE();
    if (hdma -> Instance == DMA_Channel0) {
        HAL_NVIC_SetPriority(DMA_Channel0_IRQn, 3);
        HAL_NVIC_EnableIRQ(DMA_Channel0_IRQn);
    }
    else if (hdma -> Instance == DMA_Channel1) {
        HAL_NVIC_SetPriority(DMA_Channel1_IRQn, 3);
        HAL_NVIC_EnableIRQ(DMA_Channel1_IRQn);
    }
    else if (hdma -> Instance == DMA_Channel2) {
        HAL_NVIC_SetPriority(DMA_Channel2_IRQn, 3);
        HAL_NVIC_EnableIRQ(DMA_Channel2_IRQn);
    }
    else if (hdma -> Instance == DMA_Channel3) {
        HAL_NVIC_SetPriority(DMA_Channel3_IRQn, 3);
        HAL_NVIC_EnableIRQ(DMA_Channel3_IRQn);
    }
    else if (  (hdma -> Instance == DMA_Channel4)  || (hdma -> Instance == DMA_Channel5) ||
             (hdma -> Instance == DMA_Channel6) || (hdma -> Instance == DMA_Channel7 )) 
         {
            HAL_NVIC_SetPriority(DMA_Channel4_7_IRQn, 3);
            HAL_NVIC_EnableIRQ(DMA_Channel4_7_IRQn);
         }
    

    return HAL_DMA_Init(hdma);

}

__attribute__((isr)) void DMA_Channel0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&(hdma_channel[0]));
}

__attribute__((isr)) void DMA_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&(hdma_channel[1]));
}

__attribute__((isr)) void DMA_Channel2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&(hdma_channel[2]));
}

__attribute__((isr)) void DMA_Channel3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&(hdma_channel[3]));
}

__attribute__((isr)) void DMA_Channel4_7_IRQHandler(void)
{
   HAL_DMA_IRQHandler(&(hdma_channel[4]));
   HAL_DMA_IRQHandler(&(hdma_channel[5]));
   HAL_DMA_IRQHandler(&(hdma_channel[6]));
   HAL_DMA_IRQHandler(&(hdma_channel[7]));

}
