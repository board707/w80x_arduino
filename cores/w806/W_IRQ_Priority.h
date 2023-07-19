#ifndef _W_IRQ_PRIORITY_H
#define _W_IRQ_PRIORITY_H

//#define TICK_INT_PRIORITY 0                /*!< SYS Tick Priority   - do not change it here!!    */
                                             /*! defined in /cores/w806/lib/drivers/wm_cpu.c  */

#define  DMA_Channel0_IRQn_PRIORITY  1          /*!< DMA  Channel 0 global Interrupt                      */
#define  DMA_Channel1_IRQn_PRIORITY  1          /*!< DMA  Channel 1 global Interrupt                      */
#define  DMA_Channel2_IRQn_PRIORITY  1          /*!< DMA  Channel 2 global Interrupt                      */
#define  DMA_Channel3_IRQn_PRIORITY  1          /*!< DMA  Channel 3 global Interrupt                      */
#define  DMA_Channel4_7_IRQn_PRIORITY  1        /*!< DMA  Channel 4~7 global Interrupt                    */
#define  DMA_BRUST_IRQn_PRIORITY  1             /*!< DMA Burst global Interrupt                           */

#define  I2C_IRQn_PRIORITY  1          			/*!< I2C Interrupts                                       */
#define  ADC_IRQn_PRIORITY  1         		    /*!< ADC Convert Interrupts                               */
#define  TIM_IRQn_PRIORITY  1                     /*!< Timer0~5 Trigger                                     */
#define  PWM_IRQn_PRIORITY  1                  /*!< PWM Interrupt                                        */
#define  I2S_IRQn_PRIORITY  1                  /*!< I2S  Interrupts                                      */

#define  GPIOA_IRQn_PRIORITY  1                    /*!< GPIO A Interrupt                                     */
#define  GPIOB_IRQn_PRIORITY  1                /*!< GPIO B Interrupt                                     */

#define  UART0_IRQn_PRIORITY  1                   /*!< UART0 Interrupts                                     */
#define  UART1_IRQn_PRIORITY  1                  /*!< UART1 Interrupt                                      */
#define  UART2_5_IRQn_PRIORITY  1                /*!< UART2/3/4/5 Interrupt                                */

#define  SPI_LS_IRQn_PRIORITY  1                  /*!< Low Speed SPI Interrupt                              */
#define  SPI_HS_IRQn_PRIORITY  1                  /*!< High Speed SPI Interrupt                             */
#define  SIDO_HOST_IRQn_PRIORITY  1              /*!< SDIO Host Interrupts                                 */
#define  SDIO_IRQn_PRIORITY  1                /*!< SDIO Receiver/Transmit/Command Receiver/Transmit Interrupt */


#define  TOUCH_IRQn_PRIORITY  2 					/*!< TOUCH SENSOR                                      */
#define  MAC_IRQn_PRIORITY  2                  /*!< MAC Interrupt                                        */
#define  RF_CFG_IRQn_PRIORITY  2                /*!< RF_CFG Interrupt                                        */
#define  SEC_IRQn_PRIORITY  2                   /*!< SEC RX/TX_MNGT/TX_DATA Interrupt                     */
#define  RSA_IRQn_PRIORITY  2                    /*!< RSA Interrupts                                       */
#define  CRYPTION_IRQn_PRIORITY  2                 /*!< GPSEC Interrupts                                     */
#define  BLE_IRQn_PRIORITY  2              /*!< BLE Interrupt                                        */
#define  BT_IRQn_PRIORITY  2                  /*!< BT Interrupt                                         */
#define  FLASH_IRQn_PRIORITY  2                    /*!< Flash Interrupt                                      */
#define  PMU_IRQn_PRIORITY  2                     /*!< PMU Interrupt                                        */
#define  WDG_IRQn_PRIORITY  2                     /*!< Watch Dog Interrupt                         */

#endif