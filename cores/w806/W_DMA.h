#ifndef __W_DMA_H__
#define __W_DMA_H__
/**
 * @file  W_DMA.h
 *
 * @brief  Minimal implementation of W80x DMA support
 *  
 * @author Dmitry E. Dmitriev
 *
 * Copyright (c) 2023
 */
#include "./include/driver/wm_hal.h"

#define DMA_REQUEST_SOURCE_SDIO_RX     0xFF01UL
#define DMA_REQUEST_SOURCE_SDIO_TX     0xFF02UL

extern DMA_HandleTypeDef hdma_channel[8];



#ifdef __cplusplus
extern "C"  {
#endif
    /**
     * @brief       Get number of next unused DMA channel.
     *              The W80x controllers has an eight equivalent
     *              DMA channels. Every channel can be used for
     *              every type of DMA transfers.
     *
     *
     * @return 		DMA channel num, 0-7 in success or (-1) if no free
     *              channels available.
     *
     * @note
     */
    int8_t claim_unused_dma_channel();

    /**
     * @brief       Configure DMA channel to use of some common 
     *              Memory-to-Periph or Periph-to-Memory DMA modes.
     *              The transfer direction are determined by selected mode.
     *              Use one of these modes:
     *                  DMA_REQUEST_SOURCE_SPI_RX
     *                  DMA_REQUEST_SOURCE_SPI_TX
     *                  DMA_REQUEST_SOURCE_ADC_CH0
     *                  DMA_REQUEST_SOURCE_ADC_CH1 
     *                  DMA_REQUEST_SOURCE_ADC_CH2 
     *                  DMA_REQUEST_SOURCE_ADC_CH3      
     *                  DMA_REQUEST_SOURCE_I2S_RX  
     *                  DMA_REQUEST_SOURCE_I2S_TX
     *                  DMA_REQUEST_SOURCE_SDIO_RX
     *                  DMA_REQUEST_SOURCE_SDIO_TX
     * 
     * @param[in] ch dma chammel number, byte
     * @param[in] request_src periph and direction mode
     *
     * @return 		Pointer to configured DMA handle, DMA_HandleTypeDef*
     *
     * @note
     */
    DMA_HandleTypeDef *dma_simple_config(uint8_t ch, uint32_t request_src);

    /**
     * @brief      Initialize a DMA channel with prepared configuration. 
     * 
     * @param[in] hdma pointer to DMA handle, DMA_HandleTypeDef*
     *
     * @return 		HAL_OK or some HAL_ERROR values
     *
     * @note
     */
    uint8_t dma_init(DMA_HandleTypeDef *hdma);


    /**
     * @brief       DMA IRQ Handlers
     * 
     *
     * @note
     */
    void DMA_Channel0_IRQHandler(void);
    void DMA_Channel1_IRQHandler(void);
    void DMA_Channel2_IRQHandler(void);
    void DMA_Channel3_IRQHandler(void);
    void DMA_Channel4_7_IRQHandler(void);
 
#ifdef __cplusplus
}
#endif

#endif