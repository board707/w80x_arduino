/* 
For HLK-W806-KIT
Briefly close the contacts GND and PB5, the LEDs will light up
*/
#include <stdio.h>
#include <Arduino.h>

static volatile uint8_t key_flag = 0;

static void GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  __HAL_RCC_GPIO_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(GPIOB_IRQn, 0);
  HAL_NVIC_EnableIRQ(GPIOB_IRQn);
}

void HAL_GPIO_EXTI_Callback(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin) {
  if ((GPIOx == GPIOB) && (GPIO_Pin == GPIO_PIN_5)) {
    key_flag = 1;
  }
}

void setup() {
  HAL_Init();
  GPIO_Init();
}

void loop() {
  if (key_flag == 1) {
    HAL_Delay(20);
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_RESET) {
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);
    }
    key_flag = 0;
  }
}
