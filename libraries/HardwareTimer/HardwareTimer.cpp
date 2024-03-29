#include "HardwareTimer.h"
#include "W_IRQ_Priority.h"

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    //UNUSED(htim);
    __HAL_RCC_TIM_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM_IRQn, TIM_IRQn_PRIORITY);
    HAL_NVIC_EnableIRQ(TIM_IRQn);
}



void HAL_TIM_Callback(TIM_HandleTypeDef *htim)
{
   // UNUSED(htim);
    if (timer_callback[htim->Instance] != NULL) (* (timer_callback[htim->Instance])) ();
}


TIM_HandleTypeDef* timer_devices[TIMER_COUNT] = {&htim0,&htim1,&htim2,&htim3,&htim4,&htim5};
timer_irq_callback timer_callback[TIMER_COUNT] = {0};