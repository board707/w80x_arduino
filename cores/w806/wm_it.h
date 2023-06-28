#ifndef __WM_IT_H__
#define __WM_IT_H__

void CORET_IRQHandler(void);
void GPIOA_IRQHandler(void);
void GPIOB_IRQHandler(void);
void UART0_IRQHandler(void);
void UART1_IRQHandler(void);
void UART2_5_IRQHandler(void);
void WDG_IRQHandler(void);
void TIM0_5_IRQHandler(void);
void ADC_IRQHandler(void);
void PMU_IRQHandler(void);
void TOUCH_IRQHandler(void);
void PWM_IRQHandler(void);

#endif


#if 0
/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro  def_irq_handler handler_name
    .weak   \handler_name
    .globl  \handler_name
    .set    \handler_name, Default_Handler
    .endm

    def_irq_handler tspend_handler
    def_irq_handler SDIO_IRQHandler            /*  0:  SDIO         */
    def_irq_handler MAC_IRQHandler             /*  1:  MAC          */
    def_irq_handler RF_Cfg_IRQHandler          /*  2:  RF Cfg       */
    def_irq_handler SEC_IRQHandler             /*  3:  SEC          */
    def_irq_handler DMA_Channel0_IRQHandler         /*  4:  DMA_Channel0       */
    def_irq_handler DMA_Channel1_IRQHandler         /*  5:  DMA_Channel1       */
    def_irq_handler DMA_Channel2_IRQHandler         /*  6:  DMA_Channel2       */
    def_irq_handler DMA_Channel3_IRQHandler         /*  7:  DMA_Channel3       */
    def_irq_handler DMA_Channel4_7_IRQHandler       /*  8:  DMA_Channel4_7     */
    def_irq_handler DMA_BRUST_IRQHandler            /*  9:  DMA_BRUST          */
    def_irq_handler I2C_IRQHandler            /*  10: IIC         */
    def_irq_handler ADC_IRQHandler            /*  11: SD ADC      */
    def_irq_handler SPI_LS_IRQHandler         /*  12: LS SPI      */
    def_irq_handler SPI_HS_IRQHandler         /*  13: HS SPI      */
    def_irq_handler GPIOA_IRQHandler          /*  14: GPIOA       */
    def_irq_handler GPIOB_IRQHandler          /*  15: GPIOB       */
    def_irq_handler UART0_IRQHandler          /*  16: UART0       */
    def_irq_handler UART1_IRQHandler         /*  17: UART1       */
    def_irq_handler TOUCH_IRQHandler         /*  18: TOUCH SENSOR  */
    def_irq_handler UART2_5_IRQHandler       /*  19: UART2_5    */
    def_irq_handler BLE_IRQHandler            /*  20: BLE         */
    def_irq_handler BT_IRQHandler             /*  21: BT          */
    def_irq_handler PWM_IRQHandler            /*  22: PWM         */
    def_irq_handler I2S_IRQHandler            /*  23: I2S         */
    def_irq_handler SDIO_HOST_IRQHandler      /*  24: SDIO HOST   */
    def_irq_handler CORET_IRQHandler        /*  25:  CoreTIM      */
    def_irq_handler RSA_IRQHandler          /*  26:  RSA          */
    def_irq_handler GPSEC_IRQHandler        /*  27:  GPSEC        */
    def_irq_handler FLASH_IRQHandler        /*  28:  Flash        */
    def_irq_handler PMU_IRQHandler          /*  29:  PMU          */
    def_irq_handler TIM0_5_IRQHandler       /*  30:  Timer0_5     */
    def_irq_handler WDG_IRQHandler          /*  31:  Watch dog    */
#endif