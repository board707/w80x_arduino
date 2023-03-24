/*
  For HLK-W806-KIT
*/
#include <stdio.h>
#include <Arduino.h>


// Define Timer handle
TIM_HandleTypeDef htim1;
static volatile uint8_t key_flag = 0;
void tim_irq_routine();

void Error_Handler_tim(void)
{
    while (1); 
}

// redefine system MspInit method
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    //UNUSED(htim);
    __HAL_RCC_TIM_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM_IRQn, 0);
    HAL_NVIC_EnableIRQ(TIM_IRQn);
}

//redefine system HAL_TIM_Callback method
void HAL_TIM_Callback(TIM_HandleTypeDef *htim)
{
   // UNUSED(htim);
    if (htim == &htim1) tim_irq_routine();
}

// function that will be called by timer IRQ
void tim_irq_routine() {
    
    key_flag++;
    if (key_flag % 3 == 0)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_24, (GPIO_PinState)0);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_25, (GPIO_PinState)1);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_26, (GPIO_PinState)1);
    }
    else if (key_flag % 3 == 1)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_24, (GPIO_PinState)1);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_25, (GPIO_PinState)0);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_26, (GPIO_PinState)1);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_24, (GPIO_PinState)1);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_25, (GPIO_PinState)1);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_26, (GPIO_PinState)0);
    }
}

// configure GPIOs for LEDs
static void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_24 | GPIO_PIN_25 | GPIO_PIN_26;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

// timer configuration
static void TIM1_Init(void)
{
    htim1.Instance = TIM1;
    htim1.Init.Unit = TIM_UNIT_MS;
    htim1.Init.Period = 1000;
    htim1.Init.AutoReload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
    {
        printf("TIM init error\r\n");
        Error_Handler_tim();
    }
    if (HAL_TIM_Base_Start_IT(&htim1) != HAL_OK)
    {
        printf("TIM start_it error\r\n");
        Error_Handler_tim();
    }
}

void setup() {

    printf("Led Demo\r\n");
    
    HAL_Init();
    GPIO_Init();
    TIM1_Init();
    
    printf("End setup\r\n");
    
}

void loop() {

    HAL_Delay(500);    
}