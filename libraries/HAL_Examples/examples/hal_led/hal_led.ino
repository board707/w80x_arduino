/*
  For HLK-W806-KIT
*/
#include <stdio.h>
#include <Arduino.h>

static volatile uint8_t key_flag = 0;

static void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

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
}

void setup() {
    Serial.begin(115200);
    Serial.printf("Led Demo\r\n");
    GPIO_Init();
}

void loop() {

    delay(500);
    key_flag++;
    
    if (key_flag % 3 == 0)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (GPIO_PinState)0);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (GPIO_PinState)1);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (GPIO_PinState)1);
    }
    else if (key_flag % 3 == 1)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (GPIO_PinState)1);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (GPIO_PinState)0);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (GPIO_PinState)1);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (GPIO_PinState)1);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (GPIO_PinState)1);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (GPIO_PinState)0);
    }
}
