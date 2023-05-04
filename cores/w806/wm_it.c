#include "wm_hal.h"

extern TIM_HandleTypeDef htim0;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

extern uint32_t GPIOA_CallbackFlag;
extern uint32_t GPIOB_CallbackFlag;

extern UART_HandleTypeDef huart1;

#define readl(addr) ({unsigned int __v = (*(volatile unsigned int *) (addr)); __v;})
__attribute__((isr)) void CORET_IRQHandler(void)
{
    readl(0xE000E010);
    HAL_IncTick();
}
__attribute__((isr)) void GPIOA_IRQHandler(void)
{
 	if ((GPIOA->MIS & GPIOA_CallbackFlag) != 0) {
        HAL_GPIO_EXTI_IRQHandler(GPIOA, GPIOA->MIS);
    }
    else {
        HAL_GPIO_EXTI_IRQHandler(GPIOA, GPIO_PIN_ALL);
    }
}

__attribute__((isr)) void GPIOB_IRQHandler(void)
{
	if ((GPIOB->MIS & GPIOB_CallbackFlag) != 0) {
        HAL_GPIO_EXTI_IRQHandler(GPIOB, GPIOB->MIS);
    }
    else {
        HAL_GPIO_EXTI_IRQHandler(GPIOB, GPIO_PIN_ALL);
    }
}

__attribute__((isr)) void TIM0_5_IRQHandler(void)
{
    
    HAL_TIM_IRQHandler(&htim0);
    HAL_TIM_IRQHandler(&htim1);
    HAL_TIM_IRQHandler(&htim2);
    HAL_TIM_IRQHandler(&htim3);
    HAL_TIM_IRQHandler(&htim4);
    HAL_TIM_IRQHandler(&htim5);
 }

__attribute__((isr)) void UART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}