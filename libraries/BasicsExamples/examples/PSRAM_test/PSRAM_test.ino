#include "Arduino.h"

// Tested on Air103,W806,W801 boards with APS6404L-3SQR-SN chip
// For W806,W801 use pins PB0...PB5

PSRAM_HandleTypeDef hpsram;

static void PSRAM_Init(void);


uint8_t *psram_buf ;

void setup()
{
  Serial.begin(115200);
  // setup PSRAM HEAP size
  dram_heap_init(1024 * 1024);
  
  psram_buf = (uint8_t *)dram_heap_malloc(100);
  PSRAM_Init();
}
 
 
void loop()
{
  uint8_t temp[100];
  int i = 0;

  memset(psram_buf, 0, 100);
  for(i = 0; i < 100; i++)
  {
    Serial.printf("%x ", psram_buf[i]);
  }
  Serial.printf("\r\n");

  for(i = 0; i < 100; i++)
  {
    temp[i] = i % 256;
  }

  memcpy(psram_buf, temp, 100);
  for(i = 0; i < 100; i++)
  {
    Serial.printf("%x ", psram_buf[i]);
  }
  Serial.printf("\r\n");

  delay(1000);
}

static void PSRAM_Init(void)
{
  // 0 - Air103
  // 1 - W801,W806
  HAL_PSRAM_ConfigPin(1);
  hpsram.Instance = PSRAM;
  hpsram.Init.Div = 3;
  hpsram.Init.Mode = PSRAM_MODE_QSPI;

  if (HAL_PSRAM_Init(&hpsram) != HAL_OK)
  {
    Serial.printf("Init error...\r\n");
  }
}
