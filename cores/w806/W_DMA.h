#ifndef __W_DMA_H__
#define __W_DMA_H__

#include "./include/driver/wm_hal.h"

extern DMA_HandleTypeDef hdma_channel[8];



#ifdef __cplusplus
extern "C"  {
#endif

int8_t claim_unused_dma_chan();
DMA_HandleTypeDef*  dma_simple_config(uint8_t ch, uint32_t request_src);
uint8_t dma_init(DMA_HandleTypeDef* hdma);
void DMA_Channel0_IRQHandler(void);
void DMA_Channel1_IRQHandler(void);
void DMA_Channel2_IRQHandler(void);
void DMA_Channel3_IRQHandler(void);
void DMA_Channel4_7_IRQHandler(void);
 
#ifdef __cplusplus
}
#endif

#endif