#ifndef _W_INTERRUPT_H
#define _W_INTERRUPT_H

#include <Arduino.h>
/** timer interrupt callback type **/
typedef void (*gpio_irq_callback)();

#define IS_GPIO_IT_MODE(MODE) (((MODE) == GPIO_MODE_IT_HIGH_LEVEL )     ||\
                            ((MODE) == GPIO_MODE_IT_LOW_LEVEL )      ||\
                            ((MODE) == GPIO_MODE_IT_RISING)          ||\
                            ((MODE) == GPIO_MODE_IT_FALLING)         ||\
                            ((MODE) == GPIO_MODE_IT_RISING_FALLING) )


#define RISING      GPIO_MODE_IT_RISING             
#define FALLING     GPIO_MODE_IT_FALLING           
#define CHANGE      GPIO_MODE_IT_RISING_FALLING     
#define HIGH_LEVEL  GPIO_MODE_IT_HIGH_LEVEL         
#define LOW_LEVEL   GPIO_MODE_IT_LOW_LEVEL          

#define GPIOA_PINS_CNT 16
#define GPIOB_PINS_CNT 32

/*extern uint32_t GPIOA_CallbackFlag ;
extern gpio_irq_callback GPIOA_Callback[GPIOA_PINS_CNT];

extern uint32_t GPIOB_CallbackFlag;
extern gpio_irq_callback GPIOB_Callback[GPIOB_PINS_CNT];*/

void attachInterrupt(uint16_t pin, uint16_t mode, gpio_irq_callback callback);
void detachInterrupt(uint16_t pin);

void HAL_GPIO_EXTI_Callback(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);

#endif