
#include "W_interrupt.h"
#include "W_IRQ_Priority.h"
uint32_t GPIOA_CallbackFlag =0;
gpio_irq_callback GPIOA_Callback[GPIOA_PINS_CNT] = {0};

uint32_t GPIOB_CallbackFlag =0;
gpio_irq_callback GPIOB_Callback[GPIOB_PINS_CNT] = {0};

void attachInterrupt(uint16_t pin, gpio_irq_callback callback, uint16_t mode)
{ 

    if (pin_Map[pin].ulPinAttribute & PIN_DIO_Msk)
    {
        if (IS_GPIO_IT_MODE(mode))
        {
            GPIO_InitTypeDef GPIO_InitStruct = {0};
            GPIO_InitStruct.Pin = pin_Map[pin].halPin;
            GPIO_InitStruct.Mode = mode;
            if (!is_gpio_clk_en)
            {
                __HAL_RCC_GPIO_CLK_ENABLE();
                is_gpio_clk_en = true;
            }
            HAL_GPIO_Init(pin_Map[pin].pPort, &GPIO_InitStruct);
            if (pin_Map[pin].pPort == GPIOB)
            {
                HAL_NVIC_SetPriority(GPIOB_IRQn, GPIOB_IRQn_PRIORITY); // приоритет прерывания 1
                HAL_NVIC_EnableIRQ(GPIOB_IRQn);
                GPIOB_CallbackFlag |= pin_Map[pin].halPin;
                GPIOB_Callback[pin - PB0] = callback;
            }
            else
            {
                HAL_NVIC_SetPriority(GPIOA_IRQn, GPIOA_IRQn_PRIORITY);
                HAL_NVIC_EnableIRQ(GPIOA_IRQn);
                GPIOA_CallbackFlag |= pin_Map[pin].halPin;
                GPIOA_Callback[pin - PA0] = callback;
            }
        }
    }
}

void detachInterrupt(uint16_t pin)
{

    if (pin_Map[pin].ulPinAttribute & PIN_DIO_Msk)
    {

        HAL_GPIO_DeInit(pin_Map[pin].pPort, pin_Map[pin].halPin);
        if (pin_Map[pin].pPort == GPIOB)
        {
            GPIOB_CallbackFlag &= ~(pin_Map[pin].halPin);
            GPIOB_Callback[pin - PB0] = NULL;
        }
        else
        {
            GPIOA_CallbackFlag &= ~(pin_Map[pin].halPin);
            GPIOA_Callback[pin - PA0] = NULL;
        }
    }
}


void HAL_GPIO_EXTI_Callback(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    //UNUSED(GPIO_Pin);
    uint8_t i =0;
    if (GPIO_Pin == GPIO_PIN_ALL)  return;
    if (GPIOx == GPIOA) {
        while (i < GPIOA_PINS_CNT) {
            if (GPIO_Pin & (1 << i)) {
                if (GPIOA_Callback[i] != NULL) (* (GPIOA_Callback[i])) ();
            }
            i++;
        }
    }
    else {
        while (i < GPIOB_PINS_CNT) {
            if (GPIO_Pin & (1 << i)) {
                if (GPIOB_Callback[i] != NULL) (* (GPIOB_Callback[i])) ();
            }
            i++;
        }
    }
        
    
}