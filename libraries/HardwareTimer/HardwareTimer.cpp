#include "HardwareTimer.h"

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    //UNUSED(htim);
    __HAL_RCC_TIM_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM_IRQn, 0);
    HAL_NVIC_EnableIRQ(TIM_IRQn);
}



void HAL_TIM_Callback(TIM_HandleTypeDef *htim)
{
   // UNUSED(htim);
    if (timer_callback[htim->Instance] != NULL) (* (timer_callback[htim->Instance])) ();
}