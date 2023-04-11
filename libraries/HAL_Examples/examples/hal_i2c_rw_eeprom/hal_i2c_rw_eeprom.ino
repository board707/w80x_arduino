#include "Arduino.h"

#define ADRS_BYTES 2  // Количество байт для адресации ячеек например 1-24с02,2-24с1024

I2C_HandleTypeDef hi2c;

void I2C_Init(void);
void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, char data ); 
uint8_t i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress );

#define DEVICE_ADDR 0x50 // Например в шилде TinyRTC

void setup() {

  char w_buf[] = "AT24CXX I2C WRITE THEN READ TEST";  // Записываемые данные
  uint8_t len = strlen(w_buf);

  I2C_Init();
  // Запись данных
  if (ADRS_BYTES == 2) {
    for (int i=0; i < len; i++) i2c_eeprom_write_byte(DEVICE_ADDR,i,w_buf[i]);
  } else {
    for (int i = 0; i < len; i++) HAL_I2C_Write(&hi2c, (DEVICE_ADDR << 1), i, (uint8_t *)(w_buf + i), 1);
  }
  uint8_t b;
  // Чтение данных
  if (ADRS_BYTES == 2) {
    for (int i=0; i < len; i++) {
        b = i2c_eeprom_read_byte(DEVICE_ADDR, i); 
      printf("%c",(char)b); 
    } 
    printf("\r\n"); 
  } else {
    for (int i=0; i < len; i++) {
      HAL_I2C_Read(&hi2c, (DEVICE_ADDR << 1), i, &b, 1);
      printf("%c",(char)b); 
    }
    printf("\r\n"); 
  }

}

void loop() {

}

uint8_t i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress )
{
    HAL_I2C_Write_Byte(&hi2c, ((deviceaddress << 1) & 0xFE), 1);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
    HAL_I2C_Write_Byte(&hi2c, (int)(eeaddress >> 8), 0);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
    HAL_I2C_Write_Byte(&hi2c, (int)(eeaddress & 0xFF), 0);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
    HAL_I2C_Write_Byte(&hi2c, ((deviceaddress << 1) | 0x01), 1);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
  return HAL_I2C_Read_Byte(&hi2c, 0, 1);
OUT:
    HAL_I2C_Stop(&hi2c);
    return 0;
}

void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, char data )
{
    HAL_I2C_Write_Byte(&hi2c, ((deviceaddress << 1) & 0xFE), 1);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
    HAL_I2C_Write_Byte(&hi2c, (int)(eeaddress >> 8), 0);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
    HAL_I2C_Write_Byte(&hi2c, (int)(eeaddress & 0xFF), 0);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
    HAL_I2C_Write_Byte(&hi2c, data, 0);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
OUT:
    HAL_I2C_Stop(&hi2c);
  delay(3);
}

void I2C_Init(void)
{
  __HAL_RCC_I2C_CLK_ENABLE();           // Тактирование
  __HAL_AFIO_REMAP_I2C_SCL(GPIOA, GPIO_PIN_1);  // Переключение выводов
  __HAL_AFIO_REMAP_I2C_SDA(GPIOA, GPIO_PIN_4);  // на I2C контроллер
  hi2c.Instance = I2C;
  hi2c.Frequency = 100000;            // Частота тактирования (до 1Мгц)
  HAL_I2C_Init(&hi2c);
}
