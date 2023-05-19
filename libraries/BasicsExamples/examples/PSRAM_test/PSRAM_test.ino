#include "Arduino.h"

PSRAM_HandleTypeDef hpsram;

static void PSRAM_Init(void);


static uint8_t *psram_buf = (uint8_t *)PSRAM_ADDR_START;

void setup()
{
  PSRAM_Init();
}
 
 
void loop()
{
  uint8_t temp[100];
  int i = 0;

  memset(psram_buf, 0, 100);
  for(i = 0; i < 100; i++)
  {
    printf("%x ", psram_buf[i]);
  }
  printf("\r\n");

  for(i = 0; i < 100; i++)
  {
    temp[i] = i % 256;
  }

  memcpy(psram_buf, temp, 100);
  for(i = 0; i < 100; i++)
  {
    printf("%x ", psram_buf[i]);
  }
  printf("\r\n");

  delay(1000);
}

static void PSRAM_Init(void)
{
  __HAL_RCC_PSRAM_CLK_ENABLE();
  __HAL_RCC_GPIO_CLK_ENABLE();
  // For Air103
  __HAL_AFIO_REMAP_PSRAM_CS(GPIOB, GPIO_PIN_27);
  __HAL_AFIO_REMAP_PSRAM_CLK(GPIOA, GPIO_PIN_15);
  __HAL_AFIO_REMAP_PSRAM_D0(GPIOB, GPIO_PIN_2);
  __HAL_AFIO_REMAP_PSRAM_D1(GPIOB, GPIO_PIN_3);
  __HAL_AFIO_REMAP_PSRAM_D2(GPIOB, GPIO_PIN_4);
  __HAL_AFIO_REMAP_PSRAM_D3(GPIOB, GPIO_PIN_5);

  hpsram.Instance = PSRAM;
  hpsram.Init.Div = 3;
  hpsram.Init.Mode = PSRAM_MODE_QSPI;

  if (HAL_PSRAM_Init(&hpsram) != HAL_OK)
  {
    printf("Init error...\r\n");
  }
}