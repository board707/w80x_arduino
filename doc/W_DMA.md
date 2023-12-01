## Minimal implementation of W80x DMA support
The W80x controllers has an eight equivalent DMA channels. Every channel can be used for
every type of DMA transfers. This library is intended to help the user configure the DMA for some typical Memory-to-Periph or Periph-to-Memory application modes.

### **Usage**
To add DMA functionality to your sketch you'll need a three simple steps.

#### 1. Get number of next unused DMA channel. 

**`dma_ch = claim_unused_dma_channel();`**

Returns channel number 0-7 or (-1) if no free channel available.

#### 2. Configure the channel to work with selected peripheral.

**`DMA_HandleTypeDef* hdma = dma_simple_config(dma_ch, request_src);`**

Where `dma_ch` is channel number, obtained in step 1. Parameter `request_src` should be one of:
* DMA_REQUEST_SOURCE_SPI_RX
* DMA_REQUEST_SOURCE_SPI_TX
* DMA_REQUEST_SOURCE_ADC_CH0
* DMA_REQUEST_SOURCE_ADC_CH1 
* DMA_REQUEST_SOURCE_ADC_CH2 
* DMA_REQUEST_SOURCE_ADC_CH3      
* DMA_REQUEST_SOURCE_I2S_RX  
* DMA_REQUEST_SOURCE_I2S_TX
* DMA_REQUEST_SOURCE_SDIO_RX
* DMA_REQUEST_SOURCE_SDIO_TX

The method returns a pointer to preconfigured DMA handle.

#### 3. Init the hardware with prepared configuration

**`result = dma_init(hdma);`**

The method returns `HAL_OK` if success or `HAL_ERROR` in case of any issue.

Congratulation! Now the DMA channel is configured and initialized, you can attach it to selected peripheral transfer. See the `I2S/examples/Wav_SD_Player/Wav_SD_Player.ino` sketch for example of using the DMA.


